#include <Arduino.h>
#include <ESP32Servo.h>
#include "servo_semilla.h"
#include "pines.h"

const int ANGLE_OPEN  = 90;
const int ANGLE_CLOSE = 20;
Servo gripper;    
void init_gripper() {
    gripper.attach(SERVO_PIN, 500, 2500);
    gripper.write(ANGLE_OPEN);
}

void grab_seed() {
  gripper.write(ANGLE_CLOSE);
}

void release_seed() {
  gripper.write(ANGLE_OPEN);
}