#include <Arduino.h>
#include <Wire.h>
#include "scanner.h"
#include "pines.h"

void scan_i2c() {
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.println("Scanning...");
  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.println(address, HEX);
    }
  }
  Serial.println("Done");
}