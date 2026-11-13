#include <Arduino.h>
#include <ESP32Servo.h>
#include "servo_seed.h"
#include "pines.h"
#include "settings.h"


Servo gripper;    
void init_gripper() {
    gripper.attach(SERVO_PIN, 500, 2500);
    gripper.write(settings.angleOpen);
}

void grab_seed() {
  gripper.write(settings.angleClose);
}

void release_seed() {
  gripper.write(settings.angleOpen);
}