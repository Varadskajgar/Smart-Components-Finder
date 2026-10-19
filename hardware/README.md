# Smart Components Finder — Hardware Prototype

This folder adds the first standalone ESP32 locator source. It is not connected to the website and has not been tested on physical hardware.

## Parts

**Already listed as owned:** ESP32-WROOM-32 (30-pin), RC522, one card, one key fob, 16 ordinary LEDs, two boxes, breadboard, jumper wires and USB cable.

**Buy before wiring all LEDs:**
- 2 × 74HC595 DIP-16 shift-register ICs.
- 16 × 1 kΩ resistors, one per ordinary LED.
- 2 × 100 nF ceramic bypass capacitors, one per IC across VCC/GND.
- Optional: 16-pin DIP sockets, labels, and a multimeter.

The 74HC595 chips run at 3.3 V. They are logic outputs, not high-current drivers. A 1 kΩ resistor gives approximately 1 mA with a typical red LED; brightness varies by LED color. Never connect LEDs directly to ESP32 GPIO without a resistor and do not reduce resistor values to chase brightness. TI lists roughly 7.8 mA output drive for SN74HC595; absolute maximums are not design targets. Source: https://www.ti.com/product/SN74HC595

## ESP32 to modules

| ESP32 | Connect to | Function |
|---|---|---|
| 3V3 | RC522 3.3V; both 595 pin 16 | 3.3 V logic power |
| GND | RC522 GND; both 595 pin 8 | Shared ground |
| GPIO5 | RC522 SDA/SS | Chip select |
| GPIO18 | RC522 SCK | SPI clock |
| GPIO23 | RC522 MOSI | SPI controller output |
| GPIO19 | RC522 MISO | SPI controller input |
| GPIO22 | RC522 RST | Reset |
| GPIO25 | 595 #1 pin 14 DS/SER | Serial LED data |
| GPIO26 | Both 595 pin 11 SH_CP/SRCLK | Shift clock |
| GPIO27 | Both 595 pin 12 ST_CP/RCLK | Latch |

Leave RC522 IRQ unconnected. Keep RC522 at 3.3 V; its chip is not 5 V tolerant. Do not connect any 5 V signal to ESP32 or RC522.

## 74HC595 pin wiring (DIP-16)

| Pin | First chip | Second chip |
|---|---|---|
| 1–7 and 15 (QA–QH) | Each output through its own 1 kΩ resistor to LED anode | Same, outputs 9–16 |
| 8 GND | Common GND | Common GND |
| 9 QH′ | To second chip pin 14 | Not connected |
| 10 /MR (SRCLR) | 3.3 V | 3.3 V |
| 11 SH_CP | GPIO26 | GPIO26 |
| 12 ST_CP | GPIO27 | GPIO27 |
| 13 /OE | GND | GND |
| 14 DS/SER | GPIO25 | From first chip pin 9 |
| 16 VCC | 3.3 V | 3.3 V |

For each LED: chip output → 1 kΩ resistor → long LED leg (anode); short leg / flat side (cathode) → GND. Use 16 separate resistors. Put each 100 nF capacitor close between pin 16 and pin 8. Check the notch and pin 1 on the exact IC before wiring.

## Circuit overview

    ESP32 GPIO25 -> DS pin14 [595 #1] QH' pin9 -> DS pin14 [595 #2]
    ESP32 GPIO26 -> SH_CP pin11 on both chips
    ESP32 GPIO27 -> ST_CP pin12 on both chips
    ESP32 3V3    -> VCC pin16 and /MR pin10 on both
    ESP32 GND    -> GND pin8 and /OE pin13 on both; LED cathodes

    For all 16 outputs: QA..QH -> 1kΩ ->| LED -> GND

## Physical map

The mapping is in SmartComponentsFinder/SmartComponentsFinder.ino in the parts array. It assigns LED 14 to NE555 timer IC in Container 1, Tray D / Slot 01. Physically put LED 14 at that compartment, or edit output/box/position in the array to match your boxes. The firmware has a 16-item starter map; add more records by assigning an output 0–15 and an accurate physical position. The web inventory does not sync to the device.

## Arduino IDE setup and first demonstration

1. Install the ESP32 board support in Arduino IDE and library **MFRC522 by Miguel Balboa** via Library Manager.
2. Open SmartComponentsFinder/SmartComponentsFinder.ino, choose the matching ESP32-WROOM-32 board, connect USB, and upload. Serial Monitor: 115200 baud, Newline. This sketch uses SPI pins 18/19/23 and SS 5.
3. Before assembling all 16 positions, first test one LED on GPIO32 through a 1 kΩ resistor (then cathode to GND) with a simple Blink sketch. Unplug before changing wiring.
4. Test one 595 output, with its 1 kΩ LED resistor and 100 nF capacitor. Then add the second chip and remaining 15 resistor/LED pairs. Run an output walk test; stop if anything heats or the ESP32 resets.
5. In the locator Serial Monitor type list, then NE555 timer IC (or 14). Expected message: Container 1, Tray D / Slot 01, LED output 14. Check only LED 14 blinks. Type stop; all LEDs must turn off.
6. Only after the LED test is stable, connect and test the RC522. Scan card and key fob; copy printed UIDs into CONTAINER_1_UID and CONTAINER_2_UID, respectively, then upload. Scan again and confirm recognized-container messages. Set REQUIRE_RFID_VERIFICATION to true only after both configured tags work.

Until UIDs are supplied in the sketch, RFID scans print UID and unknown-tag messages. Verification is disabled by default so offline Serial selection can demonstrate the LEDs without configured RFID. The RFID tags identify containers; they are not secure authentication credentials.

## Servo and future features

The SG90 is not wired or driven. First check the exact servo voltage and stall-current needs; use a separate suitable 5 V supply and common ground, never the ESP32 3V3 pin. Choose a free signal pin only after checking the wiring.

Future ideas: Next/Previous buttons on unused GPIO32/33 using INPUT_PULLUP; OLED over I2C (GPIO21/22 commonly used, but GPIO22 is occupied by RC522 RST); buzzer; Wi-Fi dashboard; approval before activation. They are not implemented in this prototype.

## Status and limits

Serial messages report ready state, scanned RFID UID, recognized/unknown tags, selected component and location, LED action, stop, and errors. This source is for an Arduino IDE prototype. It has not been compiled or physically tested here. The first successful demonstration requires the missing two 74HC595 chips, 16 resistors, two capacitors, assembly, and testing on your ESP32.
