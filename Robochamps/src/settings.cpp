#include <Arduino.h>
#include <Preferences.h>
#include "settings.h"

Preferences prefs;
Settings settings = { 100, 100, 0, 0, 90, 20 };

void settings_load() {
  prefs.begin("masina", false);
  settings.speedDrive = prefs.getInt("spdDrive",  settings.speedDrive);
  settings.speedTurn  = prefs.getInt("spdTurn",   settings.speedTurn);
  settings.trimLeft   = prefs.getInt("trimL",     settings.trimLeft);
  settings.trimRight  = prefs.getInt("trimR",     settings.trimRight);
  settings.angleOpen  = prefs.getInt("angOpen",   settings.angleOpen);
  settings.angleClose = prefs.getInt("angClose",  settings.angleClose);
  prefs.end();
}

void settings_save() {
  prefs.begin("masina", false);
  prefs.putInt("spdDrive", settings.speedDrive);
  prefs.putInt("spdTurn",  settings.speedTurn);
  prefs.putInt("trimL",    settings.trimLeft);
  prefs.putInt("trimR",    settings.trimRight);
  prefs.putInt("angOpen",  settings.angleOpen);
  prefs.putInt("angClose", settings.angleClose);
  prefs.end();
}