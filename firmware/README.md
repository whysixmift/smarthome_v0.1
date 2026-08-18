# ESP32 Smart Home Controller Firmware

This directory contains the production-ready C++ firmware for the **Smart Home ESP32 4-Channel Relay Controller** PCB.

## Features

* **Web Dashboard**: Built-in responsive HTML/CSS/JS control panel accessible via web browser.
* **REST API**: Simple HTTP JSON endpoints for external integration (Home Assistant, Node-RED, or custom apps).
* **Physical Switch Control**: Debounced digital input handlers for external wall switches on GPIO 13, 12, 27, and 14.
* **Non-Volatile Memory (NVS)**: Automatically remembers relay states across power loss and reboots.
* **OTA Updates**: Wireless Over-The-Air firmware flashing via Wi-Fi.
* **I2C Expansion**: Pre-configured I2C bus on GPIO 21 (SDA) and GPIO 22 (SCL) for OLED displays or environmental sensors.
* **Heartbeat Indicator**: System status LED pulse on GPIO 2.

## Hardware GPIO Mapping

| Function | ESP32 GPIO | Schematic Net | Description |
| :--- | :--- | :--- | :--- |
| **Relay 1** | GPIO 25 | `SIG_1/IO25` | Output 1 (HF46F Relay K1 + Status LED D6) |
| **Relay 2** | GPIO 23 | `SIG_2/IO23` | Output 2 (HF46F Relay K2 + Status LED D7) |
| **Relay 3** | GPIO 19 | `SIG_3/IO19` | Output 3 (HF46F Relay K3 + Status LED D8) |
| **Relay 4** | GPIO 18 | `SIG_4/IO18` | Output 4 (HF46F Relay K4 + Status LED D9) |
| **Switch 1** | GPIO 13 | `SWITCH_1/IO13` | Digital Input 1 |
| **Switch 2** | GPIO 12 | `SWITCH_2/IO12` | Digital Input 2 |
| **Switch 3** | GPIO 27 | `SWITCH_3/IO27` | Digital Input 3 |
| **Switch 4** | GPIO 14 | `SWITCH_4/IO14` | Digital Input 4 |
| **Status LED**| GPIO 2 | `LED/IO2` | Onboard Indicator LED D5 |
| **I2C SDA** | GPIO 21 | `SDA` | Expansion Header J4 Pin 3 |
| **I2C SCL** | GPIO 22 | `SCL` | Expansion Header J4 Pin 4 |

## Compilation & Flashing

### Option A: Using PlatformIO (Recommended)

1. Open the project root folder in VS Code with the PlatformIO extension installed.
2. Connect the board to your PC via a USB Type-C cable.
3. Click the **PlatformIO Upload** button (or run `pio run --target upload` in terminal).
4. Open serial monitor at `115200` baud.

### Option B: Using Arduino IDE

1. Open `firmware.ino` in Arduino IDE.
2. Select Board: **Tools > Board > ESP32 Arduino > ESP32 Dev Module**.
3. Install required libraries via Library Manager:
   - `ESPAsyncWebServer`
   - `AsyncTCP`
   - `ArduinoJson`
4. Connect the USB-C cable and click **Upload**.

## Usage & API Endpoints

Once powered, the board creates a Wi-Fi Access Point:
* **SSID**: `ESP32-SmartHome-AP`
* **Password**: `12345678`
* **Dashboard URL**: `http://192.168.4.1`

### REST Endpoints

* `GET /api/status`: Returns JSON containing current relay states, uptime, RSSI, and free heap.
* `POST /api/relay/toggle?id=0`: Toggles relay index (0 to 3).
* `POST /api/relay?id=0&state=on`: Explicitly sets relay index to `on` or `off`.
