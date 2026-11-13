#pragma once

struct Settings {
  int speedDrive;
  int speedTurn;
  int trimLeft;
  int trimRight;
  int angleOpen;
  int angleClose;
};

extern Settings settings;

void settings_load();
void settings_save();