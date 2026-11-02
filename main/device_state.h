#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#define HISTORY_MAX 512

// ---- Event group bits ----
#define EVT_WIFI_CONNECTED_BIT  (1 << 0)
#define EVT_TIME_SYNCED_BIT     (1 << 1)
#define EVT_NET_CHANGED_BIT     (1 << 2)
#define EVT_OTA_TRIGGER_BIT     (1 << 3)

// ---- Button events ----
typedef enum {
    BTN_EVT_UP,
    BTN_EVT_DOWN,
    BTN_EVT_LEFT,
    BTN_EVT_RIGHT,
    BTN_EVT_OK_SHORT,
    BTN_EVT_OK_LONG,
    BTN_EVT_UP_LONG,
    BTN_EVT_DOWN_LONG,
} button_event_t;

typedef struct {
    uint16_t interval_min;
    uint16_t interval_sec;
    uint16_t run_time_sec;
    uint32_t quantity_g;                // Total hopper capacity in GRAMS
    uint32_t consumption_per_run_g;     // grams per feed run
    uint8_t  speed_percent;
    bool     enabled;
} feed_config_t;

typedef struct {
    feed_config_t feed1;
    uint32_t today_g;
    uint32_t lifetime_g;
    uint32_t remaining_g;               // feed left in the hopper (GRAMS)

    // runtime (not persisted)
    bool     motor_on;
    bool     configuring_interval;
    bool     wifi_connected;
    float    battery_percent;
    uint32_t next_feed_remaining_sec;
    uint32_t run_time_remaining_sec;

    // "dd-Mon-yyyy:grams," records
    char     history[HISTORY_MAX];

    // OTA runtime (not persisted)
    bool      ota_in_progress;
    uint8_t   ota_progress_pct;
    esp_err_t ota_last_error;
    uint32_t  ota_last_check_epoch;
    
    // OTA version info
    bool      ota_update_available;
    char      ota_available_version[24];
    uint32_t  ota_available_size;
} device_state_t;

extern device_state_t     g_dev;
extern SemaphoreHandle_t  g_state_mutex;
extern QueueHandle_t      g_button_queue;
extern EventGroupHandle_t g_events;

#define STATE_LOCK()    xSemaphoreTake(g_state_mutex, portMAX_DELAY)
#define STATE_UNLOCK()  xSemaphoreGive(g_state_mutex)

void device_state_init(void);
void device_state_snapshot(device_state_t *out);
void device_state_roll_day(const char *date_str);

// ---- storage.c ----
void storage_init(void);
void storage_save(void);
void storage_reset(void);

// ---- tasks ----
void button_task(void *arg);
void motor_task(void *arg);
void net_task(void *arg);
void display_task(void *arg);
void ota_task(void *arg);