#ifndef buttons_H
#define buttons_H

#include "pico/stdlib.h"

#define BTN_A 14
#define BTN_B 15

void initButtons();
bool getButton(uint b);
#endif