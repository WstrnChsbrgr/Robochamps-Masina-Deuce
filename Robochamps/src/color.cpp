#include <Arduino.h>
#include "color.h"
#include "pines.h"

void Color_init() {
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);
  pinMode(OUT_PIN, INPUT);
  digitalWrite(S0_PIN, HIGH);
  digitalWrite(S1_PIN, LOW);
}

int Channel_read(int s2, int s3) {
    digitalWrite(S2_PIN, s2);
    digitalWrite(S3_PIN, s3);
    delay(20);
    return pulseIn(OUT_PIN, LOW, 50000);
}