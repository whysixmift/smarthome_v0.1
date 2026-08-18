# Smart Home ESP32 Board

I wanted to control lights and appliances in my room over Wi-Fi without having loose relay modules, breadboards, and 220V AC wires floating around my desk. Wiring loose AC mains on a breadboard felt super unsafe, so I designed this 4-channel relay board in KiCad.

![3D view](images/board_3d_full.png)
*3D render of the board in KiCad*

## Features

- **ESP32-WROOM-32**: Main microcontroller for Wi-Fi control.
- **4 Relays**: Switches 4 separate AC/DC loads (5V 5A relays driven by a ULN2003A driver chip).
- **Onboard Power**: Hi-Link HLK-PM01 takes 100-240V AC input and outputs 5V DC, so I don't need an external power adapter. An AMS1117-3.3 steps it down to 3.3V for the ESP32.
- **USB-C Flashing**: Built-in CH340C serial chip with auto-reset circuit so code uploads straight over USB-C.
- **Wall Switch Inputs**: Screw terminal `J3` lets me plug in external wall switches.
- **I2C Header**: 4-pin header `J4` for extra I2C sensors if needed later.

## Pin Map

| ESP32 Pin | Connected To | What It Does |
| :--- | :--- | :--- |
| GPIO 25 | Relay 1 + LED D6 | Relay 1 Output |
| GPIO 23 | Relay 2 + LED D7 | Relay 2 Output |
| GPIO 19 | Relay 3 + LED D8 | Relay 3 Output |
| GPIO 18 | Relay 4 + LED D9 | Relay 4 Output |
| GPIO 13 | Terminal J3 Pin 1 | Wall Switch 1 Input |
| GPIO 12 | Terminal J3 Pin 2 | Wall Switch 2 Input |
| GPIO 27 | Terminal J3 Pin 3 | Wall Switch 3 Input |
| GPIO 14 | Terminal J3 Pin 4 | Wall Switch 4 Input |
| GPIO 21 | Header J4 Pin 3 | I2C SDA |
| GPIO 22 | Header J4 Pin 4 | I2C SCL |
| GPIO 2 | LED D5 | Status LED |
| GPIO 0 | Button SW2 | Boot button |
| EN | Button SW1 | Reset button |

## Board Pictures

### Schematic
![Schematic](images/schematic.png)
*KiCad schematic*

### PCB
![PCB Layout](images/pcb_layout.png)
*PCB layout*

### Details
![LEDs](images/board_3d_corner_detail.png)
*Status LEDs and passives*

![ESP32](images/board_3d_esp32_detail.png)
*ESP32 and USB-C section*

## Firmware

Firmware is inside the `firmware/` folder. It uses PlatformIO with Arduino framework and simple FreeRTOS tasks to read the wall switches and toggle relays.

To flash:
1. Open `firmware/` in VS Code with PlatformIO.
2. Plug in the board over USB-C.
3. Run `pio run -t upload`.

## BOM & Cost

Full component list with purchase links is in [`BOM.csv`](BOM.csv). Total cost is around **$9.36**.

| Part | Qty | Link |
| :--- | :---: | :--- |
| ESP32-WROOM-32 | 1 | [LCSC C82899](https://www.lcsc.com/product-detail/WiFi-Modules_Espressif-Systems-ESP32-WROOM-32-N4_C82899.html) |
| HF46F 5V Relays | 4 | [LCSC C38573](https://www.lcsc.com/product-detail/Relays_Hongfa-America-HF46F-005-HS1_C38573.html) |
| ULN2003A Driver | 1 | [LCSC C14881](https://www.lcsc.com/product-detail/Darlington-Transistor-Arrays_STMicroelectronics-ULN2003D1013TR_C14881.html) |
| HLK-PM01 AC-DC | 1 | [LCSC C209743](https://www.lcsc.com/product-detail/Power-Modules_Hi-Link-HLK-PM01_C209743.html) |
| CH340C Serial | 1 | [LCSC C84681](https://www.lcsc.com/product-detail/USB-Drivers-Receivers-Transceivers_WCH-Jiangsu-Qinheng-Microelec-CH340C_C84681.html) |
| USB-C Connector | 1 | [LCSC C165948](https://www.lcsc.com/product-detail/USB-Connectors_HCTL-HC-TYPE-C-16P-01A_C165948.html) |
| Screw Terminals | 2 | [LCSC C8456](https://www.lcsc.com/product-detail/Pluggable-System-Terminal-Block_C8456.html) |

## Notes / Honest Status

- All ERC and DRC checks in KiCad pass without errors, and firmware builds fine with `pio run`.
- I haven't tested the firmware on the physical PCB yet since the board hasn't arrived.
- Be careful with AC mains (100-240V) near `PS1` and terminal `J2`. Keep it in an insulated case.
