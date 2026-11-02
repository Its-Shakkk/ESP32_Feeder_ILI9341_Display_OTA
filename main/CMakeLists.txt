idf_component_register(
    SRCS
        "main.c"
        "device_state.c"
        "storage.c"
        "lcd_ili9341.c"
        "motor.c"
        "button_task.c"
        "motor_task.c"
        "net_task.c"
        "display_task.c"
        "http_client.c"
        "ota_manager.c"
        "rollback_manager.c"
        "nvs_storage.c"
        "ota_task.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES
        app_update
        driver
        esp_timer
        esp_wifi
        esp_netif
        esp_event
        nvs_flash
        lwip
        esp_http_client
        json
        mbedtls
)

target_add_binary_data(${COMPONENT_LIB} "../ota_public.pem" TEXT)