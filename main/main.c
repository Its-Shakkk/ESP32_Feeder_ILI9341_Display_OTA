#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "app_config.h"
#include "device_state.h"
#include "lcd_ili9341.h"
#include "motor.h"
#include "nvs_storage.h"
#include "rollback_manager.h"
#include "ota_task.h"

static const char *TAG = "feeder";

void app_main(void)
{
    ESP_LOGI(TAG, "Firmware Feeder v%s", APP_FW_VERSION);

    // 1. Shared RTOS objects (mutex, button queue, event group)
    device_state_init();

    // 2. Persistent config (also inits NVS flash — do NOT call nvs_storage_init())
    storage_init();
    nvs_print_ota_info();        // read-only diagnostics from "ota" namespace
    rollback_manager_startup();  // logs running partition + OTA image state

    // 3. Hardware
    motor_hw_init();
    lcd_init();

    // 4. Tasks
    xTaskCreate(button_task,  "button_task",  BUTTON_TASK_STACK,  NULL, BUTTON_TASK_PRIO,  NULL);
    xTaskCreate(motor_task,   "motor_task",   MOTOR_TASK_STACK,   NULL, MOTOR_TASK_PRIO,   NULL);
    xTaskCreate(net_task,     "net_task",     NET_TASK_STACK,     NULL, NET_TASK_PRIO,     NULL);
    xTaskCreate(display_task, "display_task", DISPLAY_TASK_STACK, NULL, DISPLAY_TASK_PRIO, NULL);
    xTaskCreate(ota_task,     "ota_task",     OTA_TASK_STACK,     NULL, OTA_TASK_PRIO,     NULL);
}