#include <SPI.h>
#include <MFRC522.h>
constexpr uint8_t RFID_SS=5, RFID_RST=22;
constexpr uint8_t SR_DATA=25, SR_CLOCK=26, SR_LATCH=27;
// Scan each tag, copy its uppercase UID here, then upload again.
const char CONTAINER_1_UID[]="";
const char CONTAINER_2_UID[]="";
constexpr bool REQUIRE_RFID_VERIFICATION=false; // enable after setting/testing UIDs
MFRC522 rfid(RFID_SS,RFID_RST);
struct Part { const char* name; uint8_t output; uint8_t box; const char* position; };
// Output is 0..15 in code and LED 1..16 in Serial Monitor.
const Part parts[]={
 {"220 ohm resistor",0,1,"Tray A / Slot 01"},{"1k ohm resistor",1,1,"Tray A / Slot 02"},
 {"4.7k ohm resistor",2,1,"Tray A / Slot 03"},{"10k ohm resistor",3,1,"Tray A / Slot 04"},
 {"100k ohm resistor",4,1,"Tray A / Slot 05"},{"100uF capacitor",5,1,"Tray B / Slot 01"},
 {"10uF capacitor",6,1,"Tray B / Slot 02"},{"100nF ceramic capacitor",7,1,"Tray B / Slot 03"},
 {"red LED",8,1,"Tray C / Slot 01"},{"green LED",9,1,"Tray C / Slot 02"},
 {"blue LED",10,1,"Tray C / Slot 03"},{"1N4007 diode",11,1,"Tray C / Slot 04"},
 {"1N4148 diode",12,1,"Tray C / Slot 05"},{"NE555 timer IC",13,1,"Tray D / Slot 01"},
 {"LM358 op amp",14,1,"Tray D / Slot 02"},{"ESP32 development board",15,2,"Pocket 01"}
};
const size_t PART_COUNT=sizeof(parts)/sizeof(parts[0]);
int selected=-1, verifiedBox=0; bool blinking=false,ledOn=false;
uint32_t lastBlink=0,lastScan=0; const uint32_t BLINK_MS=500, RFID_DEBOUNCE_MS=1200;
void writeOutputs(uint16_t bits){ digitalWrite(SR_LATCH,LOW); shiftOut(SR_DATA,SR_CLOCK,MSBFIRST,(uint8_t)(bits>>8)); shiftOut(SR_DATA,SR_CLOCK,MSBFIRST,(uint8_t)bits); digitalWrite(SR_LATCH,HIGH); }
void stopLocator(){ blinking=false; ledOn=false; selected=-1; writeOutputs(0); Serial.println("[STOP] All locator LEDs off."); }
int findPart(String q){ q.trim(); q.toLowerCase(); for(size_t i=0;i<PART_COUNT;i++){String n=parts[i].name;n.toLowerCase();if(q==n||q==String(i+1))return (int)i;} if(q=="ne555"||q=="555")return 13; return -1; }
void selectPart(int i){ if(i<0||i>=(int)PART_COUNT){Serial.println("[ERROR] Unknown component. Type list.");return;} const Part&p=parts[i]; if(REQUIRE_RFID_VERIFICATION&&verifiedBox!=p.box){Serial.printf("[ERROR] Scan configured tag for Container %u first.\n",p.box);return;} selected=i;blinking=true;ledOn=true;lastBlink=millis();writeOutputs((uint16_t)1<<p.output); Serial.printf("[SELECTED] %s | Container %u | %s | LED output %u\n",p.name,p.box,p.position,p.output+1);Serial.println("[LED] Assigned position blinking. Type stop to turn off."); }
void listParts(){Serial.println("Enter list number or component name:");for(size_t i=0;i<PART_COUNT;i++)Serial.printf("%u) %s | C%u | %s | LED %u\n",(unsigned)(i+1),parts[i].name,parts[i].box,parts[i].position,parts[i].output+1);Serial.println("Commands: list, status, stop");}
String uidString(){String s;for(byte i=0;i<rfid.uid.size;i++){if(rfid.uid.uidByte[i]<0x10)s+="0";s+=String(rfid.uid.uidByte[i],HEX);}s.toUpperCase();return s;}
void scanTag(){if(!rfid.PICC_IsNewCardPresent()||!rfid.PICC_ReadCardSerial())return;String uid=uidString();Serial.print("[RFID] UID: ");Serial.println(uid);if(strlen(CONTAINER_1_UID)&&uid==CONTAINER_1_UID){verifiedBox=1;Serial.println("[RFID] Recognized Container 1.");}else if(strlen(CONTAINER_2_UID)&&uid==CONTAINER_2_UID){verifiedBox=2;Serial.println("[RFID] Recognized Container 2.");}else Serial.println("[RFID] Unknown/unconfigured tag; copy UID into sketch.");lastScan=millis();rfid.PICC_HaltA();rfid.PCD_StopCrypto1();}
void command(String s){s.trim();String q=s;q.toLowerCase();if(!q.length())return;if(q=="stop"||q=="off"||q=="x")stopLocator();else if(q=="list"||q=="help"||q=="?"){listParts();Serial.println("Type NE555 timer IC or 14 for demo.");}else if(q=="status")Serial.printf("[STATUS] selected=%s, verified container=%d\n",selected<0?"none":parts[selected].name,verifiedBox);else{int i=findPart(s);if(i<0)Serial.println("[ERROR] Component not found. Type list.");else selectPart(i);}}
void setup(){Serial.begin(115200);pinMode(SR_DATA,OUTPUT);pinMode(SR_CLOCK,OUTPUT);pinMode(SR_LATCH,OUTPUT);writeOutputs(0);SPI.begin(18,19,23,RFID_SS);rfid.PCD_Init();delay(50);Serial.println("\nSmart Components Finder - offline locator");Serial.println("[READY] 115200 baud. Type list, then NE555 timer IC. Type stop.");Serial.println("[READY] RC522 initialized; scan card/key fob to print UID.");Serial.println("[READY] LEDs off.");if(!CONTAINER_1_UID[0]||!CONTAINER_2_UID[0])Serial.println("[NOTE] RFID UID mapping not configured yet.");}
void loop(){if(Serial.available())command(Serial.readStringUntil('\n'));if(millis()-lastScan>=RFID_DEBOUNCE_MS)scanTag();if(blinking&&millis()-lastBlink>=BLINK_MS){lastBlink=millis();ledOn=!ledOn;writeOutputs(ledOn?((uint16_t)1<<parts[selected].output):0);}}
