#include <Arduino.h>
#include "indicador.h"
#include "color.h"
#include "config.h"
#include "control.h"
#include "pines.h"

const unsigned long DETECT_EVERY_MS = 300;
const unsigned long HOLD_MS = 2500;

unsigned long lastDetect = 0;
unsigned long shownAt = 0;
int shownColor = COLOR_NONE;

void show_color(int color) {
    digitalWrite(LED_R_PIN, LOW);
    digitalWrite(LED_G_PIN, LOW);
    digitalWrite(LED_B_PIN, LOW);
    digitalWrite(LED_Y_PIN, LOW);
    switch (color) {
      case COLOR_RED:
        digitalWrite(LED_R_PIN, HIGH);
        break;
      case COLOR_GREEN:
        digitalWrite(LED_G_PIN, HIGH);
        break;
      case COLOR_BLUE:
        digitalWrite(LED_B_PIN, HIGH);
        break;
      case COLOR_YELLOW:
        digitalWrite(LED_Y_PIN, HIGH);
        break;
      case COLOR_NONE:
             break;
    }
}

void init_indicador() {
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  pinMode(LED_Y_PIN, OUTPUT);
  show_color(COLOR_NONE);
}

void update_indicador() {
  unsigned long now = millis();
  if (shownColor != COLOR_NONE && now - shownAt > HOLD_MS) {
    shownColor = COLOR_NONE;
    show_color(COLOR_NONE);
  }

  if (is_moving()) {
    return;
  }
  if (now - lastDetect < DETECT_EVERY_MS) {
    return;
  }

  int c = detect_color();
  lastDetect = millis();
  if (c != COLOR_NONE) {
    shownColor = c;
    shownAt = millis();
    show_color(c);
  }
}