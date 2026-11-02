# ESP32 Feeder – ILI9341 Display with OTA

## Overview

ESP32-based automatic feeder firmware with an ILI9341 TFT display, motor control, persistent configuration storage, and Over-The-Air (OTA) firmware update support.

The project is developed using the **ESP-IDF framework**.

## Features

- ILI9341 TFT display interface
- Button-based user interaction
- Motor control task
- Persistent configuration using NVS
- HTTP client communication
- OTA firmware update management
- Firmware manifest generation
- OTA rollback management
- Custom flash partition table

## Project Structure

```text
ESP32_Feeder_ILI9341_Display_OTA/
│
├── main/
│   ├── app_config.h
│   ├── button_task.c
│   ├── device_state.c
│   ├── device_state.h
│   ├── display_task.c
│   ├── font5x7.h
│   ├── http_client.c
│   ├── http_client.h
│   ├── lcd_ili9341.c
│   ├── lcd_ili9341.h
│   ├── main.c
│   ├── manifest.h
│   ├── motor_task.c
│   ├── nvs_storage.c
│   ├── nvs_storage.h
│   ├── ota_manager.c
│   ├── ota_manager.h
│   ├── ota_task.c
│   ├── ota_task.h
│   ├── rollback_manager.c
│   ├── rollback_manager.h
│   └── storage.c
│
├── tools/
│   └── make_manifest.py
│
├── CMakeLists.txt
├── partitions.csv
├── sdkconfig.defaults
├── ota_public.pem
├── build_and_publish.bat
└── README.md
```

*The tree above is based on the current project files. Update it if your source structure changes.*

## Requirements

- ESP32 development board compatible with the project configuration
- ILI9341 TFT display
- Motor driver and feeder motor
- Push buttons
- ESP-IDF development environment
- Python 3

## Build Instructions

1. Install and configure ESP-IDF.
2. Open the ESP-IDF terminal.
3. Navigate to the project directory.
4. Configure the target chip if required.
5. Build the firmware.

```bash
idf.py set-target esp32
idf.py build
```

Flash the firmware:

```bash
idf.py -p COMx flash monitor
```

Replace `COMx` with your actual ESP32 serial port.

## OTA Update

The project contains OTA-related modules for firmware update management, manifest handling, and rollback management.

The OTA workflow, server configuration, firmware signing procedure, and deployment instructions should be documented here as the implementation is finalized.

## Security

- Never commit `ota_private.pem` or any private signing key.
- Keep Wi-Fi passwords, API tokens, and server credentials out of source control.
- The public verification key may be included if required by the firmware.
- Do not commit local build output or backup configuration files.

## Configuration

Review local configuration before committing `sdkconfig`. Use `sdkconfig.defaults` for project defaults that are safe to share.

## License

Add a license if you intend to distribute this project publicly. Otherwise, specify your intended usage and distribution terms.
