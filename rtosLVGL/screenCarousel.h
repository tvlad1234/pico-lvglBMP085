#ifndef _SCREENCAROUSEL_H
#define _SCREENCAROUSEL_H

#include "lvgl.h"

void addToCarousel(lv_obj_t *s);
void nextScreen();
void prevScreen();
void goToScreen(uint i);

#endif