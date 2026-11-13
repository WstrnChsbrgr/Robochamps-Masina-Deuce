#include <Arduino.h>
#include "motores.h"
#include "pines.h"
#include "settings.h"

void init_motors() {
    pinMode(MOTORR_IN1_PIN, OUTPUT);
    pinMode(MOTORR_IN2_PIN, OUTPUT);
    pinMode(MOTORL_IN1_PIN, OUTPUT);
    pinMode(MOTORL_IN2_PIN, OUTPUT);
    pinMode(STBY_PIN, OUTPUT);
    digitalWrite(STBY_PIN, HIGH);
    ledcAttach(MOTORL_PWM_PIN, 1000, 8);
    ledcAttach(MOTORR_PWM_PIN, 1000, 8);
}

void motor_move(int motor, int speed) {
  int trim = (motor == 0) ? settings.trimLeft : settings.trimRight;
  speed = speed * (100 + trim) / 100;

  int pinIn1, pinIn2, pinPwm;
  if (motor == 0) {
    pinIn1 = MOTORL_IN1_PIN;
    pinIn2 = MOTORL_IN2_PIN;
    pinPwm = MOTORL_PWM_PIN;  
  } else {
    pinIn1 = MOTORR_IN1_PIN;
    pinIn2 = MOTORR_IN2_PIN;
    pinPwm = MOTORR_PWM_PIN;
  }

  if (speed > 0) {
    digitalWrite(pinIn1, HIGH);
    digitalWrite(pinIn2, LOW);
  } else if (speed < 0) {
    digitalWrite(pinIn1, LOW);
    digitalWrite(pinIn2, HIGH);
  } else {
    digitalWrite(pinIn1, LOW);
    digitalWrite(pinIn2, LOW);
  }
  ledcWrite(pinPwm, constrain(abs(speed), 0, 255));
}


void forward(int speed) {
  motor_move(0, speed);
  motor_move(1, speed);
}

void turn(int speed) {
  motor_move(0, speed);
  motor_move(1, -speed);
}

void stop() {
  motor_move(0, 0);
  motor_move(1, 0);
}