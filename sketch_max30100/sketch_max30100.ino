#include <M5CoreS3.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

const uint32_t kIntervalTimeMilliSec = 1000;
PulseOximeter pox;

void setup() {
  M5.begin();

  // I2C(PortA) Power On
  M5.Axp.setBoostBusOutEn(true);
  Wire.begin(2, 1);

  M5.Lcd.clear(BLACK);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(4);

  // Initialize sensor
  if (!pox.begin()) {
    M5.Lcd.println("FAILED");
    for(;;);
  }

  //LED Configuration
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}

void loop() {
  static uint32_t tsLastReport = 0;

  pox.update(); //update pulse oximeter
  if (millis() - tsLastReport > kIntervalTimeMilliSec) {
    auto heart_rate = (int)pox.getHeartRate();
    auto spo2 = pox.getSpO2();
    Display(heart_rate, spo2);
    tsLastReport = millis();
  }
}

void Display(int heart_rate, int spo2) {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0,70);
  M5.Lcd.printf("HR:   %3d", heart_rate);
  M5.Lcd.setCursor(0,110);
  M5.Lcd.printf("SPO2: %3d", spo2);
}
