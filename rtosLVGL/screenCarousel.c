#include "pico/stdlib.h"
#include "lvgl.h"
#include "driv_lvgl.h"
#include "screenCarousel.h"
#define NUM_SCR_MAX 10

int numScreens = 0;
int currentScreen = 0;
lv_obj_t *screenCarousel[NUM_SCR_MAX];

void addToCarousel(lv_obj_t *s)
{
    screenCarousel[numScreens++] = s;
}

void nextScreen()
{
    if (currentScreen < numScreens - 1)
    {

#ifdef USE_FREERTOS
        takeLvglSemaphore();
        currentScreen++;
#endif
        lv_scr_load_anim(screenCarousel[currentScreen], LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0, false);
#ifdef USE_FREERTOS
        giveLvglSemaphore();
#endif
    }
}

void prevScreen()
{
    if (currentScreen > 0)
    {

#ifdef USE_FREERTOS
        takeLvglSemaphore();
        currentScreen--;
#endif
        lv_scr_load_anim(screenCarousel[currentScreen], LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, false);
#ifdef USE_FREERTOS
        giveLvglSemaphore();
#endif
    }
}

void goToScreen(uint i)
{
    if (i < numScreens)
#ifdef USE_FREERTOS
        takeLvglSemaphore();
#endif
    lv_scr_load_anim(screenCarousel[i], LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
#ifdef USE_FREERTOS
    giveLvglSemaphore();
#endif
}
