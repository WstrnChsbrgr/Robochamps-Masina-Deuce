#pragma once

struct ColorRef { int r, g, b; };

const int COLOR_NONE   = 0;
const int COLOR_RED    = 1;
const int COLOR_GREEN  = 2;
const int COLOR_BLUE   = 3;
const int COLOR_YELLOW = 4;

const ColorRef REFS[5] = {
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 }
};

const int MAX_DISTANCE = 0;