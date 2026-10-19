# Smart Components Finder

A responsive component inventory website plus a separate standalone ESP32 hardware prototype. The physical locator can be selected over USB Serial and runs without the website or internet. The website and firmware do not communicate with each other.

## Website

Open index.html directly, or run python -m http.server 8000 from the repository folder and visit http://localhost:8000. Enable free GitHub Pages in repository Settings → Pages, select Deploy from a branch, then main and root.

Edit the sample component array in index.html to adjust names, quantities and website locations. Inventory quantities are examples. The website is static; the ESP32 is not connected to it.

## Standalone ESP32 locator

- Firmware: hardware/SmartComponentsFinder/SmartComponentsFinder.ino
- Pin wiring, required parts, safety notes and beginner test steps: hardware/README.md
- RFID UID placeholder mapping: hardware/rfid-mapping.json

The starter firmware maps NE555 timer IC to Container 1, Tray D / Slot 01, LED output 14. It supports selection and stop through Arduino IDE Serial Monitor at 115200 baud. Install the ESP32 Arduino board support and MFRC522 library by Miguel Balboa. Scan the card and key fob to read UIDs, add them to the sketch, and upload again to identify container tags.

## Parts still required for the 16-LED version

The listed owned parts do not include the LED control ICs, current-limit resistors or bypass capacitors. Obtain 2 × 74HC595 DIP-16, 16 × 1 kΩ resistors and 2 × 100 nF capacitors before wiring all 16 LEDs. Use only one resistor per LED. Keep all logic at 3.3 V; never feed 5 V into the RC522 or ESP32 pins. The SG90 is not connected or driven.

The source has not been compiled or tested on physical hardware. Follow hardware/README.md in order: one LED, one shift-register output, all 16 LEDs, then RFID.

## Future work

A Next/Previous switch pair, OLED, buzzer, Wi-Fi dashboard integration, and approval before activation are not implemented. RFID UIDs are blank until read from the user's tags. No live hardware or website status is claimed.
