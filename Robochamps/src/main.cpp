#include <Arduino.h>
#include "pines.h"
#include "motores.h"
#include "color.h"
#include "control.h"
#include "control.h" 
#include "scanner.h"
#define MODE_COLOR   1
#define MODE_WIFI    2
#define MODE_I2C     3
#define MODE_MOTORS  4
#define TEST_MODE MODE_MOTORS
int channel_red, channel_green, channel_blue;

int contador = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
    #if TEST_MODE == MODE_I2C
    scan_i2c();
  #endif
  init_controller();
  pinMode(LED_PIN, OUTPUT);
  init_motors();
  delay(5000);
  Serial.println("Ahi va weyes");
  Color_init();
  init_controller();
}

void loop() {
  #if TEST_MODE == MODE_COLOR
  channel_red = Channel_read(LOW, LOW);
  channel_green = Channel_read(HIGH, HIGH);
  channel_blue = Channel_read(LOW, HIGH);
  Serial.print("R: ");
  Serial.print(channel_red);
  Serial.print(" G: ");
  Serial.print(channel_green);
  Serial.print(" B: ");
  Serial.println(channel_blue);
  delay(500);
  #elif TEST_MODE == MODE_WIFI
  atenderControl();
  #elif TEST_MODE == MODE_MOTORS
    Serial.print("Ciclo número ");
    Serial.println(contador);
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
    contador++;
    forward(100);
    delay(2000);
    stop();
    delay(1000);
    turn(100);
    delay(2000);
    stop();
    delay(1000);  
  #endif
}