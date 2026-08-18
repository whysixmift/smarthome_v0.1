# ESP32 Smart Home Controller Firmware

This directory contains the firmware for the **Smart Home ESP32 4-Channel Relay Controller** PCB.

## Features

* **4-Channel Relay Control**: Controls four 5V mechanical relays on GPIO 25, 23, 19, and 18 via ULN2003A driver.
* **External Switch Handling**: Debounced digital input handlers for wall switches connected to screw terminal `J3` (GPIO 13, 12, 27, 14).
* **FreeRTOS Multi-Tasking**: Separate tasks for input debouncing and status LED heartbeat animation.
* **Serial Debugging**: Clean boot sequence messages at 115200 baud.

## Hardware GPIO Mapping

| Function | ESP32 GPIO | Net Name | Description |
| :--- | :--- | :--- | :--- |
| **Relay 1** | GPIO 25 | `SIG_1/IO25` | Output 1 (Relay K1 + Status LED D6) |
| **Relay 2** | GPIO 23 | `SIG_2/IO23` | Output 2 (Relay K2 + Status LED D7) |
| **Relay 3** | GPIO 19 | `SIG_3/IO19` | Output 3 (Relay K3 + Status LED D8) |
| **Relay 4** | GPIO 18 | `SIG_4/IO18` | Output 4 (Relay K4 + Status LED D9) |
| **Switch 1** | GPIO 13 | `SWITCH_1/IO13` | Terminal Block J3 Pin 1 |
| **Switch 2** | GPIO 12 | `SWITCH_2/IO12` | Terminal Block J3 Pin 2 |
| **Switch 3** | GPIO 27 | `SWITCH_3/IO27` | Terminal Block J3 Pin 3 |
| **Switch 4** | GPIO 14 | `SWITCH_4/IO14` | Terminal Block J3 Pin 4 |
| **Status LED**| GPIO 2 | `LED/IO2` | Onboard Indicator LED D5 |

## Compilation & Flashing

### Using PlatformIO

```bash
pio run -t upload
```

### Using Arduino IDE

1. Open `firmware.ino` in Arduino IDE.
2. Select Board: **ESP32 Dev Module**.
3. Click **Upload**.
