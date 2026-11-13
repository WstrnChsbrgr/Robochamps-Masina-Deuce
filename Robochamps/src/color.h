#pragma once
void color_init();
int Channel_read(int s2, int s3);
int detect_color();
bool calibrate_color(int id);
void read_rgb(int* r, int* g, int* b);