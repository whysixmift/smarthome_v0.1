# Smart Home ESP32 Board

I built this board because I wanted a single compact PCB to control lights and appliances in my room using an ESP32. Before this, I had separate relay modules, breadboards, and loose jumper wires hooked up to AC mains, which was messy and unsafe.

This project combines an AC-DC power supply, four 5V relays, an I2C expansion header, status LEDs, and USB-C flashing onto one board.

![Full 3D Render](images/board_3d_full.png)
*3D view of the smart home PCB rendered in KiCad.*

---

## What It Has Onboard

* **ESP32-WROOM-32**: Main microcontroller for Wi-Fi, Bluetooth, and logic.
* **4 Mechanical Relays**: Hongfa HF46F / 5V 5A mini power relays (`K1`-`K4`) driven by a ULN2003A Darlington transistor array.
* **I2C Header**: 4-pin header (`J4`) breaking out 5V, GND, SDA (GPIO 21), and SCL (GPIO 22) for external sensors or displays.
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
| **GPIO 21** | I2C Header | `SDA` | I2C Data (J4 Header Pin 3) |
| **GPIO 22** | I2C Header | `SCL` | I2C Clock (J4 Header Pin 4) |
| **GPIO 13** | Switch Input 1 | `SWITCH_1/IO13` | Screw Terminal J3 Pin 1 |
| **GPIO 12** | Switch Input 2 | `SWITCH_2/IO12` | Screw Terminal J3 Pin 2 |
| **GPIO 27** | Switch Input 3 | `SWITCH_3/IO27` | Screw Terminal J3 Pin 3 |
| **GPIO 14** | Switch Input 4 | `SWITCH_4/IO14` | Screw Terminal J3 Pin 4 |
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

## Assembly Order

If you're soldering this board yourself, build it in order of height:

1. **SMD ICs & Diodes**: Solder small ICs first (CH340C, ULN2003A, AMS1117-3.3, SS8050 transistors, TVS diodes, and SS34 diodes).
2. **SMD Passives**: Solder 0603 and 1206 resistors, capacitors, and status LEDs.
3. **USB-C Connector**: Solder the surface-mount USB Type-C port (`J1`).
4. **Tactile Buttons**: Solder the Reset (`SW1`) and Boot (`SW2`) switches.
5. **ESP32 Module**: Solder the ESP32-WROOM-32 castellated pads.
6. **Through-Hole Components**:
   * Solder the 4-pin I2C pin header (`J4`).
   * Solder the four 5V relays (`K1`-`K4`).
   * Solder the 6-pin screw terminal blocks (`J2`, `J3`).
   * Solder the HLK-PM01 AC-DC power module (`PS1`).

---

## Firmware

The code in `firmware/` uses PlatformIO with the Arduino framework and FreeRTOS tasks:

1. **Input Task**: Debounces physical switch inputs (`SWITCH_1`..`SWITCH_4` on `J3`) to toggle relays.
2. **Status Task**: Blinks onboard status LED `D5` as a heartbeat animation.
3. **Relay Control**: Drives ULN2003A inputs low or high to safely trigger relay coils.

### How to Flash

1. Open `firmware/` in VS Code with PlatformIO.
2. Connect the board to your computer with USB-C.
3. Run:
   ```bash
   pio run -t upload
   ```

---

## Bill of Materials (BOM)

The full BOM with quantities and purchase links is available in [`BOM.csv`](BOM.csv) and [`BOM.md`](BOM.md).

| Component | Qty | Specification | Purchase Link | Total ($) |
| :--- | :---: | :--- | :--- | :---: |
| ESP32-WROOM-32 | 1 | ESP32-WROOM-32-N4 | [LCSC C82899](https://www.lcsc.com/product-detail/WiFi-Modules_Espressif-Systems-ESP32-WROOM-32-N4_C82899.html) | $2.75 |
| Relays (Hongfa/Songle) | 4 | HF46F / 5V 5A Mini Power Relay | [LCSC C38573](https://www.lcsc.com/product-detail/Relays_Hongfa-America-HF46F-005-HS1_C38573.html) | $1.80 |
| Relay Driver IC | 1 | ULN2003A SOIC-16 | [LCSC C14881](https://www.lcsc.com/product-detail/Darlington-Transistor-Arrays_STMicroelectronics-ULN2003D1013TR_C14881.html) | $0.18 |
| Tactile Switches | 2 | 3x6mm SMD Push Button | [LCSC C318884](https://www.lcsc.com/product-detail/Tactile-Switches_C318884.html) | $0.10 |
| AC-DC Module | 1 | HLK-PM01 5V 3W | [LCSC C209743](https://www.lcsc.com/product-detail/Power-Modules_Hi-Link-HLK-PM01_C209743.html) | $2.80 |
| **TOTAL** | | | | **$9.36** |

---

## Known Issues & Notes

* **Physical Board Testing**: The PCB files pass all KiCad DRC and ERC checks and the firmware compiles with zero errors under PlatformIO (`pio run`), but I have not physically uploaded this firmware to the manufactured board yet.
* **AC Mains Handling**: 100V-240V AC connects directly to `PS1` and terminal `J2`. Be careful when handling power and use a 3D-printed enclosure.

---

## Credits

* **KiCad Libraries**: Symbols and footprints for ESP32, CH340C, ULN2003A, and passive parts.
