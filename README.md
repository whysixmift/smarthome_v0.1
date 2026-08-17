# Smart Home ESP32 Controller

This project is a custom all-in-one smart home automation PCB powered by an ESP32-WROOM-32 module. It combines an onboard AC-DC step-down power supply, four 5V relays driven by a ULN2003A driver, USB-C programming via a CH340C bridge, status LEDs, and an external I2C header into a single compact board.

![Full 3D Render](images/board_3d_full.png)
*Full 3D view of the smart home PCB rendered in KiCad.*

## Why I Made This

I started this project because I wanted to control lights and appliances in my house over Wi-Fi without dealing with a messy breadboard full of loose wires, separate relay modules, and plug-in power adapters. Having AC mains wired to loose modules on a workbench felt unsafe and clunky.

I designed this board in KiCad so I could connect AC mains directly to an onboard power converter, switch four separate AC/DC circuits using built-in relays, and flash the ESP32 directly through USB-C.

## How It Works

The hardware is split into five functional sections:

1. **Power Supply**: AC mains connects to a 5.08mm screw terminal feeding a Hi-Link HLK-PM01 step-down module, which converts 100V-240V AC to 5V DC. An AMS1117-3.3 linear regulator converts the 5V line down to 3.3V DC to power the ESP32. An SS34 Schottky diode provides reverse voltage protection.
2. **Microcontroller**: An ESP32-WROOM-32 handles Wi-Fi connectivity and logic. It has dedicated Reset (`SW1`) and Boot (`SW2`) tactile buttons for manual control.
3. **USB & Programming Circuit**: A USB Type-C port (`J1`) connects to a CH340C USB-to-UART bridge IC. Dual SS8050 NPN transistors (`Q1`, `Q2`) control the automatic DTR/RTS reset circuit so code uploads work without needing to manually press buttons.
4. **Relay Control & Driver**: To drive the relays safely without overloading the ESP32 GPIOs, the ESP32 controls a ULN2003A Darlington transistor array driver. The driver triggers four Hongfa HF46F 5V relays (`K1`-`K4`). Each relay coil is paired with a parallel SS34 flyback diode (`D10`-`D13`) to protect against inductive voltage spikes.
5. **Expansion & Status**: A 4-pin 2.54mm header (`J4`) breaks out 5V, GND, SDA, and SCL for external I2C devices like OLED displays or sensors. Six SMD LEDs (`D5`-`D9`, `D14`) give visual status for power and relay channels.

## Project Visuals

### Schematic
![Schematic Diagram](images/schematic.png)
*Schematic diagram exported from KiCad.*

### PCB Layout
![PCB Layout](images/pcb_layout.png)
*2-layer PCB trace routing and silkscreen layout.*

### Detail Renders
![Silkscreen and LED Detail](images/board_3d_corner_detail.png)
*Close-up 3D render showing status LEDs, capacitors, and silkscreen text.*

![ESP32 and USB-C Detail](images/board_3d_esp32_detail.png)
*Close-up 3D render showing the ESP32 module, USB-C connector, reset switches, and I2C header.*

## Hardware

* **Microcontroller**: ESP32-WROOM-32 (Wi-Fi + BLE)
* **Relays**: 4 x HF46F-005-HS1 (5V coils, 5A switching rating)
* **Relay Driver**: ULN2003A Darlington Transistor Array (SOIC-16)
* **Power Supply**: HLK-PM01 AC-DC module (100-240V AC to 5V DC) + AMS1117-3.3 LDO
* **USB Interface**: USB-C receptacle + CH340C USB-to-UART converter with auto-reset circuit
* **Expansion**: 4-pin I2C pin header (5V, GND, SDA, SCL)

## Assembly

For soldering this board, components should be assembled in order of height:

1. **SMD ICs & Semiconductors**: Solder small ICs first (CH340C, ULN2003A, AMS1117-3.3, SS8050 transistors, TVS diodes, and SS34 diodes).
2. **SMD Passives**: Solder 0603 and 1206 resistors, capacitors, and status LEDs.
3. **USB-C Port**: Solder the surface-mount USB-C receptacle (`J1`).
4. **Tactile Switches**: Solder the Reset (`SW1`) and Boot (`SW2`) buttons.
5. **ESP32 Module**: Solder the ESP32-WROOM-32 castellated pads onto the board.
6. **Through-Hole Modules**:
   * Solder the 4-pin I2C pin header (`J4`).
   * Solder the four HF46F relays (`K1`-`K4`).
   * Solder the 6-pin screw terminal blocks (`J2`, `J3`).
   * Solder the HLK-PM01 AC-DC power module (`PS1`).

## Flashing the ESP32

Firmware code for this project is currently not included in this repository.

When uploading code using Arduino IDE, ESP-IDF, or PlatformIO:

1. **Software & Drivers**: Install the CH340 USB-to-UART driver for your operating system.
2. **Connection**: Connect the PCB to your computer with a USB Type-C cable.
3. **Board Settings**:
   * Select **ESP32 Dev Module** in Arduino IDE or PlatformIO.
   * Select the COM / TTY serial port for the CH340 device.
4. **Uploading**: Click Upload. The onboard CH340C auto-reset circuit will pulse DTR/RTS to put the ESP32 into bootloader mode automatically.

## Bill of Materials (BOM)

| Reference | Component | Package / Footprint | Quantity | Function |
| :--- | :--- | :--- | :---: | :--- |
| `U4` | ESP32-WROOM-32 | RF_Module:ESP32-WROOM-32 | 1 | Microcontroller (Wi-Fi / BLE) |
| `PS1` | HLK-PM01 | Converter_ACDC:HLK-PMxx | 1 | AC-DC 5V step-down power supply module |
| `U3` | AMS1117-3.3 | SOT-223-3 | 1 | 3.3V LDO linear regulator |
| `U1` | ULN2003A | SOIC-16 | 1 | Darlington transistor array relay driver |
| `U2` | CH340C | SOIC-16 | 1 | USB-to-UART serial converter |
| `K1, K2, K3, K4` | HF46F_005-HS1 | RELAY_HF46F_005-HS1 | 4 | 5V 5A mini power relays |
| `J1` | USB_C_Receptacle_USB2.0_16P | USB-C 16-pin SMD | 1 | USB-C power & serial connection |
| `J2, J3` | Screw_Terminal_01x06 | TerminalBlock 6-pin 5.08mm | 2 | Relay contacts and AC input terminals |
| `J4` | Conn_01x04_Pin | PinHeader 1x4 2.54mm | 1 | I2C expansion header (5V, GND, SDA, SCL) |
| `Q1, Q2` | SS8050 | SOT-23 | 2 | Auto-reset circuit transistors |
| `D1, D2, D4` | LESD5D5.0CT1G | SOD-523 / TVS | 3 | ESD protection diodes |
| `D3, D10-D13` | SS34 | SMA | 5 | Schottky diodes (reverse power & flyback) |
| `D5-D9, D14` | LED | LED_0603 | 6 | Status indicator LEDs |
| `SW1, SW2` | SW_Push (SKRK) | SMD Pushbutton | 2 | Reset and Boot tactile switches |
| `C1, C3, C8-C12` | 0.1uF | 0603 SMD | 7 | Decoupling capacitors |
| `C2, C5, C7, C13` | 10uF | 1206 SMD | 4 | Filter capacitors |
| `C4` | 10uF / 25V | 0603 SMD | 1 | Power line capacitor |
| `C6` | 10nF | 1206 SMD | 1 | Noise suppression capacitor |
| `R1, R9, R12` | 0Ω | 0603 SMD | 3 | Zero-ohm bridge jumpers |
| `R2, R3` | 5.1kΩ | 0603 SMD | 2 | USB-C CC pull-down resistors |
| `R4-R8, R17, R20` | 2.2kΩ | 0603 SMD | 7 | LED current-limiting resistors |
| `R10, R11, R13-R16, R18, R19` | 10kΩ | 0603 SMD | 8 | Pull-up / pull-down resistors |

## Known Issues

* **AC Power Safety**: High-voltage AC mains connects to `PS1` and terminal blocks `J2`/`J3`. High-voltage clearance must be maintained when mounting inside an enclosure.
* **Firmware State**: Software/firmware code files are not yet included in this repository.

## Credits

* **Hongfa**: Symbol and footprint references for HF46F relays.
* **Hi-Link**: Symbol and footprint references for the HLK-PM01 AC-DC power module.
* **KiCad Libraries**: Standard KiCad symbol and footprint libraries for ESP32, CH340C, ULN2003A, and passive components.
