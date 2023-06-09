#pragma once
#include <stdint.h>

extern void init_videomode();
extern void set_videomode(int mode);
extern void set_pixel(int x, int y, uint32_t pixel);
extern void getinfo_videomodes(int collumns);
extern void getinfo_framebuffer();
extern void clear_screen();