#include <Arduino.h>
#include "pines.h"
#include "motores.h"

int contador = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  pinMode(LED_PIN, OUTPUT);
  init_motors();
  delay(5000);
  Serial.println("Ahi va weyes");
}

void loop() {
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
}