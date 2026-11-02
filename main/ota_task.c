#include <time.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_err.h"
#include "app_config.h"
#include "device_state.h"
#include "http_client.h"
#include "rollback_manager.h"
#include "nvs_storage.h"
#include "ota_task.h"

static const char *TAG = "ota_task";

#define OTA_BOOT_STABILITY_MS   30000ULL            // 30 s after boot before self-test
#define OTA_FIRST_CHECK_MS      (5ULL * 60 * 1000)  // 5 min after boot for first check
#define OTA_DAILY_INTERVAL_MS   (24ULL * 3600 * 1000)

/* Feeder-specific self-test run once on PENDING_VERIFY.
 * Generous by design: a router offline should not brick a good image. */
static bool feeder_self_test(void)
{
    device_state_t s;
    device_state_snapshot(&s);

    if (s.feed1.speed_percent < 10 || s.feed1.speed_percent > 100) {
        ESP_LOGE(TAG, "self-test: invalid speed %d", s.feed1.speed_percent);
        return false;
    }
    if (!s.wifi_connected) {
        ESP_LOGW(TAG, "self-test: WiFi not connected (allowed, device still functional)");
    }
    ESP_LOGI(TAG, "self-test: PASSED");
    return true;
}

static void do_ota_check(void)
{
    STATE_LOCK();
    bool connected = g_dev.wifi_connected;
    STATE_UNLOCK();

    if (!connected) {
        ESP_LOGW(TAG, "WiFi offline, skipping OTA check");
        STATE_LOCK();
        g_dev.ota_last_error = ESP_ERR_INVALID_STATE;
        STATE_UNLOCK();
        return;
    }

    STATE_LOCK();
    g_dev.ota_in_progress  = true;
    g_dev.ota_progress_pct = 0;
    g_dev.ota_last_error   = ESP_OK;
    
    // Reset version info for the new check
    g_dev.ota_update_available = false;
    g_dev.ota_available_size = 0; 
    memset(g_dev.ota_available_version, 0, sizeof(g_dev.ota_available_version));
    STATE_UNLOCK();

    // On successful OTA this never returns — the device reboots.
    esp_err_t err = http_check_version();

    STATE_LOCK();
    g_dev.ota_in_progress      = false;
    g_dev.ota_last_error       = err;
    g_dev.ota_last_check_epoch = (uint32_t)time(NULL);
    STATE_UNLOCK();

    ESP_LOGI(TAG, "OTA check result: %s", esp_err_to_name(err));
}

void ota_task(void *arg)
{
    // ── Phase 1: boot validation ─────────────────────────────────────────
    if (rollback_manager_is_pending_verify()) {
        ESP_LOGW(TAG, "PENDING_VERIFY — waiting %llu ms stability window",
                 OTA_BOOT_STABILITY_MS);
        vTaskDelay(pdMS_TO_TICKS(OTA_BOOT_STABILITY_MS));

        if (feeder_self_test()) {
            rollback_manager_confirm_image();
            nvs_clear_failed_version();
            nvs_reset_failed_count();
            ESP_LOGI(TAG, "Image confirmed VALID");
        } else {
            ESP_LOGE(TAG, "Self-test FAILED — rolling back");
            rollback_manager_handle_failed_image();  // reboots
        }
    }

    // ── Phase 2: daily + manual + MQTT trigger loop ─────────────────────
    TickType_t next_daily = xTaskGetTickCount() + pdMS_TO_TICKS(OTA_FIRST_CHECK_MS);
    ESP_LOGI(TAG, "OTA trigger loop started (daily / menu / MQTT)");

    for (;;) {
        TickType_t now = xTaskGetTickCount();
        TickType_t wait = (next_daily > now) ? (next_daily - now) : 0;

        EventBits_t bits = xEventGroupWaitBits(
            g_events, EVT_OTA_TRIGGER_BIT,
            pdTRUE,   // clear bit on exit
            pdFALSE,  // wait for any
            wait);

        if (bits & EVT_OTA_TRIGGER_BIT) {
            ESP_LOGI(TAG, "OTA triggered (manual / MQTT)");
            do_ota_check();
            // Keep the daily timer running — manual triggers don't reset it
        } else {
            ESP_LOGI(TAG, "OTA triggered (daily timer)");
            next_daily = xTaskGetTickCount() + pdMS_TO_TICKS(OTA_DAILY_INTERVAL_MS);
            do_ota_check();
        }
    }
}