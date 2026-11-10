#include <Arduino.h>
#include "color.h"
#include "pines.h"
#include "config.h"

ColorRef refs[5];
bool filled[5] = { false, false, false, false, false };
int maxDistance = 0;
bool calibrated = false;

void color_init() {
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

int read_average(int s2, int s3) {
  int sum = 0;
  for (int i = 0; i < 3; i++) {
    int value = Channel_read(s2, s3);
    if (value == 0) {
      return 0;
    }
    sum += value;
  }
  return sum / 3;
}

int detect_color() {
  int r = read_average(LOW, LOW);
  int g = read_average(HIGH, HIGH);
  int b = read_average(LOW, HIGH);

  if (r == 0 || g == 0 || b == 0) {
    return COLOR_NONE;
  }

  int best = 0;
  int bestDistance = 32000;

  for (int i = 0; i < 5; i++) {
    int distance = abs(r - refs[i].r) + abs(g - refs[i].g) + abs(b - refs [i].b);
    if (distance < bestDistance) {
      bestDistance = distance;
      best = i;
    }
  }
  if (bestDistance > maxDistance || !calibrated) {
    return COLOR_NONE;
  }
  return best;
}


void calibrate_color(int id) {
  int r = read_average(LOW, LOW);
  int g = read_average(HIGH, HIGH);
  int b = read_average(LOW, HIGH);

  if (r == 0 || g == 0 || b == 0) {
    return;
  }

  refs[id].r = r;
  refs[id].g = g;
  refs[id].b = b;
  filled[id] = true;

  if (filled[0] && filled[1] && filled[2] && filled[3] && filled[4]) {
    int smallest = 32000;
    for (int i = 0; i < 5; i++) {
      for (int j = i + 1; j < 5; j++) {
        int distance = abs(refs[i].r - refs[j].r) + abs(refs[i].g - refs[j].g) + abs(refs[i].b - refs[j].b);
        if (distance < smallest) {
          smallest = distance;
        }
      }
    }
    maxDistance = smallest / 2;
    calibrated = true;
  }
}