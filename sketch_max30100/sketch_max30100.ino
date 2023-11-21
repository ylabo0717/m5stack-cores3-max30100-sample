#include <M5CoreS3.h>

#include "MAX30100.h"

#define SAMPLING_RATE MAX30100_SAMPRATE_100HZ
#define IR_LED_CURRENT MAX30100_LED_CURR_24MA
#define RED_LED_CURRENT MAX30100_LED_CURR_27_1MA
#define PULSE_WIDTH MAX30100_SPC_PW_1600US_16BITS
#define HIGHRES_MODE true

MAX30100 sensor;

void setup() {
  M5.begin();

  // Initialize I2C
  M5.Axp.setBoostBusOutEn(true);
  Wire.begin(2, 1);

  Serial.print("Initializing MAX30100..");

  while (!sensor.begin()) {  // Initialize the sensor.
    M5.Lcd.setTextFont(4);
    M5.Lcd.setCursor(50, 100, 4);
    M5.Lcd.println("Sensor not found");
    delay(1000);
  }
  M5.Lcd.fillScreen(BLACK);
  // Set up the wanted parameters.
  sensor.setMode(MAX30100_MODE_SPO2_HR);
  sensor.setLedsCurrent(IR_LED_CURRENT, RED_LED_CURRENT);
  sensor.setLedsPulseWidth(PULSE_WIDTH);
  sensor.setSamplingRate(SAMPLING_RATE);
  sensor.setHighresModeEnabled(HIGHRES_MODE);
}

void loop() {
  uint16_t ir, red;
  sensor.update();
  while (sensor.getRawValues(&ir, &red)) {
    M5.Lcd.setTextFont(4);
    M5.Lcd.setCursor(100, 100, 4);
    M5.Lcd.printf("IR:%d               ", ir);
    M5.Lcd.setCursor(100, 130, 4);
    M5.Lcd.printf("RED:%d              ", red);
  }
}