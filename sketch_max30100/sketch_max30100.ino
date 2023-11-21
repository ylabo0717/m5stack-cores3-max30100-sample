#include <M5CoreS3.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define kIntervalTimeMilliSec     1000

PulseOximeter pox;
uint8_t Heart_rate = 0;
uint8_t Spo2 = 0;
uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected() {
  printHRandSPO2(true);
}

void setup() {
  Serial.begin(115200); // to PC via USB


  M5.begin();

  // I2C(PortA) Power On
  M5.Axp.setBoostBusOutEn(true);
  Wire.begin(2, 1);

  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextSize(4);

  // Initialize sensor
  if (!pox.begin()) {
    M5.Lcd.println("FAILED");
    for(;;);
   } else {
    M5.Lcd.println("SUCCESS");
  }

  //LED Configuration
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  //Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update(); //update pulse oximeter

  if (millis() - tsLastReport > kIntervalTimeMilliSec) {
    Heart_rate = (int)pox.getHeartRate();
    Spo2 = pox.getSpO2();
    printHRandSPO2(false);
    tsLastReport = millis();
  }
}

void printHRandSPO2(bool beat) {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(4);
  M5.Lcd.setCursor(0,70);
  M5.Lcd.print("HR:   "); M5.Lcd.println(Heart_rate);
  M5.Lcd.print("SPO2: "); M5.Lcd.println(Spo2);  
}
