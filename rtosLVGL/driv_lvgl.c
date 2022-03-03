#include "pico/stdlib.h"

#include "st7735.h"
#include "driv_lvgl.h"
#include "lvgl.h"
#include "lv_conf.h"

#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#endif

/////////Tick stuff

#ifndef USE_FREERTOS
bool tickTimerCallback(struct repeating_timer *t)
{
    lv_tick_inc(TICKS_MS);
    return true;
}
#else

void vApplicationTickHook(void)
{
    lv_tick_inc(portTICK_PERIOD_MS);
}

SemaphoreHandle_t lvglSemaphore;
TaskHandle_t lvglTask;
void createLvglTask(int priority)
{
    lvglSemaphore = xSemaphoreCreateBinary();
    xTaskCreate(lvglHandler, "lvglTask", 16384, NULL, priority, &lvglTask);
}

void takeLvglSemaphore()
{
    xSemaphoreTake(lvglSemaphore, 0);
}

void giveLvglSemaphore()
{
    xSemaphoreGive(lvglSemaphore);
}

void lvglHandler(void *p)
{
    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
        takeLvglSemaphore();
        lv_task_handler();
        giveLvglSemaphore();
    }
}
#endif

//////Display stuff
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    LCD_WriteBitmap(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, color_p);
    lv_disp_flush_ready(disp);
}

extern uint16_t _width;
extern uint16_t _height;

void initDisplayDriv()
{
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf1[128 * 160 / SCREEN_PORTION]; /*Declare a buffer for 1/10 screen size*/

    static lv_disp_drv_t disp_drv;                                            /*Descriptor of a display driver*/
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, 128 * 160 / SCREEN_PORTION); /*Initialize the display buffer.*/
    lv_disp_drv_init(&disp_drv);                                              /*Basic initialization*/
    disp_drv.flush_cb = my_disp_flush;                                        /*Set your driver function*/
    disp_drv.draw_buf = &draw_buf;                                            /*Assign the buffer to the display*/
    disp_drv.hor_res = _width;                                                /*Set the horizontal resolution of the display*/
    disp_drv.ver_res = _height;                                               /*Set the vertical resolution of the display*/
    lv_disp_drv_register(&disp_drv);                                          /*Finally register the driver*/
}

////Init all
void initLVGL()
{
#ifndef USE_FREERTOS
    static struct repeating_timer tickTimer;
    add_repeating_timer_ms(TICKS_MS, tickTimerCallback, NULL, &tickTimer);

#else
    createLvglTask(2);
#endif

    lv_init();
    initDisplayDriv();
}