#pragma once

// =========================================================================
// Feeder + OTA configuration
// =========================================================================

// --- Firmware identity (keep in sync with sdkconfig CONFIG_APP_PROJECT_VER) ---
#define APP_FW_VERSION      "4.0.0"
#define APP_DEVICE_NAME     "ESP32_FEEDER"

// --- Display Resolution (landscape) ---
#define LCD_H_RES       320
#define LCD_V_RES       240
#define LCD_MADCTL      0x40

// --- TFT pins (SPI3 / VSPI) ---
#define PIN_TFT_SCK     18
#define PIN_TFT_MISO    -1      // write-only
#define PIN_TFT_MOSI    23
#define PIN_TFT_CS      22
#define PIN_TFT_DC      2
#define PIN_TFT_RST     21
#define PIN_TFT_BLK     5
#define LCD_SPI_HOST    SPI3_HOST
#define LCD_SPI_HZ      (40 * 1000 * 1000)

// --- Buttons (active low, internal pull-up) ---
#define PIN_BTN_UP      14
#define PIN_BTN_DOWN    26
#define PIN_BTN_LEFT    13
#define PIN_BTN_RIGHT   32
#define PIN_BTN_OK      27

// --- Motors ---
#define PIN_MOTOR1      4       // Relay ON/OFF
#define PIN_MOTOR2      15      // PWM controlled

#define MOTOR2_PWM_FREQ_HZ        5000
#define MOTOR2_SHUTDOWN_DELAY_MS  2000

// --- Task parameters ---
#define BUTTON_TASK_PRIO    4
#define MOTOR_TASK_PRIO     5
#define NET_TASK_PRIO       3
#define DISPLAY_TASK_PRIO   3
#define OTA_TASK_PRIO       2       // below net/display — never starves the UI

#define BUTTON_TASK_STACK   2048
#define MOTOR_TASK_STACK    4096
#define NET_TASK_STACK      6144
#define DISPLAY_TASK_STACK  6144
#define OTA_TASK_STACK      10240   // TLS + RSA-2048 + HTTP

#define BUTTON_POLL_MS      20
#define MOTOR_TICK_MS       1000
#define DISPLAY_TICK_MS     100
#define BUTTON_DEBOUNCE_MS  50
#define BUTTON_LONG_MS      1000
#define BUTTON_QUEUE_LEN    16

// --- Feed limits ---
#define FEED_QTY_MAX_KG         100
#define FEED_RUN_MAX_SEC        60
#define FEED_CONSUM_MAX_G       100000

// --- WiFi / SNTP ---
// --- WiFi / SNTP ---
#define CONFIG_FEEDER_WIFI_SSID "YOUR_WIFI_SSID"
#define CONFIG_FEEDER_WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define CONFIG_FEEDER_NTP_SERVER "pool.ntp.org"
#define CONFIG_FEEDER_TIMEZONE "IST-5:30"


// =========================================================================
// OTA configuration
// =========================================================================
#define MANIFEST_URL "http://YOUR_OTA_SERVER/manifest.json"
#define OTA_SERVER_CERT         ""          // empty = plain HTTP (Phase 2)
#define OTA_MANIFEST_MAX_SIZE   2048
#define MAX_FAILED_ATTEMPTS     2
