#include <Arduino.h>
#include "pines.h"
#include "motores.h"
#include "color.h"
#include "control.h"
#include "scanner.h"
#include "servo_seed.h"
#include "indicador.h"
#include "settings.h"
#define MODE_COLOR   1
#define MODE_WIFI    2
#define MODE_I2C     3
#define MODE_MOTORS  4
#define MODE_SERVO   5
#define MODE_DRIVE   6
#define TEST_MODE MODE_DRIVE
int channel_red, channel_green, channel_blue;

int contador = 0;

void setup() {
  Serial.begin(115200);
  settings_load();
    #if TEST_MODE != MODE_DRIVE
    delay(4000);
  #endif  
  #if TEST_MODE == MODE_I2C
    scan_i2c();
  #endif
  init_controller();
  init_motors();
  color_init();
  init_gripper();
  init_indicador();
  pinMode(LED_PIN, OUTPUT);
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
  int detected = detect_color();
  Serial.print("  -> color: ");
  Serial.println(detected);
  delay(500);
  #elif TEST_MODE == MODE_WIFI
  attend_controller();
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
  
#elif TEST_MODE == MODE_SERVO
    grab_seed();
    delay(2000);
    release_seed();
    delay(2000);
  #elif TEST_MODE == MODE_DRIVE
    attend_controller();
    update_indicador();
  #endif
}