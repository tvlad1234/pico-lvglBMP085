#ifndef driv_lvgl_H
#define driv_lvgl_H

#define USE_FREERTOS 1

#define TICKS_MS 1
#define SCREEN_PORTION 3

#ifndef USE_FREERTOS
bool tickTimerCallback(struct repeating_timer *t);
#else
void lvglHandler(void *p);
void createLvglTask(int priority);

void takeLvglSemaphore();
void giveLvglSemaphore();

#endif

void initLVGL();

#endif