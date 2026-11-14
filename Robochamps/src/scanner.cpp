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

void test_mpu() {
  static bool woken = false;
  if (!woken) {
    Wire.beginTransmission(0x68);
    Wire.write(0x6B);
    Wire.write(0);                  // wake the sensor up
    Wire.endTransmission();
    woken = true;
  }
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);                 // first data register
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 14);
  int16_t ax = (Wire.read() << 8) | Wire.read();
  int16_t ay = (Wire.read() << 8) | Wire.read();
  int16_t az = (Wire.read() << 8) | Wire.read();
  Wire.read(); Wire.read();         // skip temperature
  int16_t gx = (Wire.read() << 8) | Wire.read();
  int16_t gy = (Wire.read() << 8) | Wire.read();
  int16_t gz = (Wire.read() << 8) | Wire.read();
  Serial.printf("acc %d %d %d   gyro %d %d %d\n", ax, ay, az, gx, gy, gz);
}