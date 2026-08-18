# Smart Home ESP32 Board

I built this board because I wanted a single compact PCB to control lights and appliances in my room using an ESP32. Before this, I had separate relay modules, breadboards, and loose jumper wires hooked up to AC mains, which was messy and unsafe.

This project combines an AC-DC power supply, four 5V relays, an OLED display header, status LEDs, and USB-C flashing onto one board.

![Full 3D Render](images/board_3d_full.png)
*3D render of the board in KiCad.*

---

## What It Has Onboard

* **ESP32-WROOM-32**: Main microcontroller for Wi-Fi, Bluetooth, and logic.
* **4 Mechanical Relays**: Hongfa HF46F / 5V 5A mini power relays (`K1`-`K4`) driven by a ULN2003A Darlington transistor array.
* **OLED Display Header**: 4-pin header (`J4`) for an SSD1306 128x64 I2C display module.
* **External Switch Inputs**: Screw terminal `J3` connecting external wall switches to GPIOs 13, 12, 27, and 14.
* **Onboard Tactile Buttons**:
  * `SW1`: Reset switch
  * `SW2`: Bootloader switch
* **AC-DC Power Supply**: Hi-Link HLK-PM01 module converts 100V-240V AC mains to 5V DC, followed by an AMS1117-3.3 regulator for 3.3V power.
* **USB-C Flashing**: USB-C port (`J1`) with a CH340C serial chip and auto-reset circuit so code uploads directly from your computer.

---

## Hardware Pinout Map

| ESP32 Pin | Device / Component | Net Name | Purpose |
| :--- | :--- | :--- | :--- |
| **GPIO 25** | Relay 1 + LED D6 | `SIG_1/IO25` | Output 1 |
| **GPIO 23** | Relay 2 + LED D7 | `SIG_2/IO23` | Output 2 |
| **GPIO 19** | Relay 3 + LED D8 | `SIG_3/IO19` | Output 3 |
| **GPIO 18** | Relay 4 + LED D9 | `SIG_4/IO18` | Output 4 |
| **GPIO 21** | OLED Display | `SDA` | I2C Data (J4 Header Pin 3) |
| **GPIO 22** | OLED Display | `SCL` | I2C Clock (J4 Header Pin 4) |
| **GPIO 13** | Switch Input 1 | `SWITCH_1/IO13` | Screw Terminal J3 Pin 1 / Switch 1 |
| **GPIO 12** | Switch Input 2 | `SWITCH_2/IO12` | Screw Terminal J3 Pin 2 / Switch 2 |
| **GPIO 27** | Switch Input 3 | `SWITCH_3/IO27` | Screw Terminal J3 Pin 3 / Switch 3 |
| **GPIO 14** | Switch Input 4 | `SWITCH_4/IO14` | Screw Terminal J3 Pin 4 / Switch 4 |
| **GPIO 2** | Status LED D5 | `LED/IO2` | Heartbeat LED |
| **GPIO 0** | Switch SW2 | `IO0` | Boot button |
| **EN** | Switch SW1 | `EN` | Reset button |

---

## Pictures

### Schematic
![Schematic Diagram](images/schematic.png)
*Schematic overview created in KiCad.*

### PCB Layout
![PCB Layout](images/pcb_layout.png)
*2-layer PCB layout.*

### Detail Views
![LED Detail](images/board_3d_corner_detail.png)
*Close-up of status LEDs and passives.*

![ESP32 Detail](images/board_3d_esp32_detail.png)
*Close-up of the ESP32, USB-C port, and reset buttons.*

---

## Firmware

The code in `firmware/` uses PlatformIO with the Arduino framework and FreeRTOS tasks:

1. **Display Task**: Periodically updates the SSD1306 128x64 OLED screen showing current relay status and uptime.
2. **Input Task**: Debounces physical switch inputs (`SWITCH_1`..`SWITCH_4` on `J3`) to toggle relays.
3. **Relay Control**: Safely drives the ULN2003A inputs low or high to switch relay coils.

### How to Flash

1. Open `firmware/` in VS Code with PlatformIO.
2. Connect the board to your computer with USB-C.
3. Run:
   ```bash
   pio run -t upload
   ```

---

## Bill of Materials (BOM)

The full BOM with quantities and purchase links is available in [`BOM.csv`](BOM.csv).

| Component | Qty | Specification | Purchase Link | Total ($) |
| :--- | :---: | :--- | :--- | :---: |
| ESP32-WROOM-32 | 1 | ESP32-WROOM-32-N4 | [LCSC C82899](https://www.lcsc.com/product-detail/WiFi-Modules_Espressif-Systems-ESP32-WROOM-32-N4_C82899.html) | $2.75 |
| Relays (Hongfa/Songle) | 4 | HF46F / 5V 5A Mini Power Relay | [LCSC C38573](https://www.lcsc.com/product-detail/Relays_Hongfa-America-HF46F-005-HS1_C38573.html) | $1.80 |
| Relay Driver IC | 1 | ULN2003A SOIC-16 | [LCSC C14881](https://www.lcsc.com/product-detail/Darlington-Transistor-Arrays_STMicroelectronics-ULN2003D1013TR_C14881.html) | $0.18 |
| OLED Display | 1 | 0.96" 128x64 I2C SSD1306 | [LCSC C2836248](https://www.lcsc.com/product-detail/OLED-Displays-Modules_KMR-KMR-0-96-OLED-W_C2836248.html) | $1.85 |
| Tactile Switches | 2 | 3x6mm SMD Push Button | [LCSC C318884](https://www.lcsc.com/product-detail/Tactile-Switches_C318884.html) | $0.10 |
| AC-DC Module | 1 | HLK-PM01 5V 3W | [LCSC C209743](https://www.lcsc.com/product-detail/Power-Modules_Hi-Link-HLK-PM01_C209743.html) | $2.80 |
| **TOTAL** | | | | **$11.21** |

---

## Known Issues & Notes

* **Physical Board Testing**: The PCB files pass all KiCad DRC and ERC checks and the firmware compiles with zero errors under PlatformIO (`pio run`), but I have not physically uploaded this firmware to the manufactured board yet.
* **AC Mains Handling**: 100V-240V AC connects directly to `PS1` and terminal `J2`. Be careful when handling power and use a 3D-printed enclosure.

---

## Credits

* **KiCad Libraries**: Symbols and footprints for ESP32, CH340C, ULN2003A, and passive parts.
* **Adafruit GFX / SSD1306**: OLED display libraries.
