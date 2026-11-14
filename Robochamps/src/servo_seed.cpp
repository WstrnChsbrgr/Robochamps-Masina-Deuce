#include <Arduino.h>
#include "servo_seed.h"
#include "pines.h"
#include "settings.h"

const int SERVO_FREQ = 50;
const int SERVO_BITS = 14;

static void servo_write(int angle) {
  angle = constrain(angle, 0, 180);
  int us = map(angle, 0, 180, 500, 2500);
  int duty = us * (1 << SERVO_BITS) / 20000;
  ledcWrite(SERVO_PIN, duty);
}

void init_gripper() {
  ledcAttach(SERVO_PIN, SERVO_FREQ, SERVO_BITS);
  servo_write(settings.angleOpen);
}

void grab_seed() {
  servo_write(settings.angleClose);
}

void release_seed() {
  servo_write(settings.angleOpen);
}