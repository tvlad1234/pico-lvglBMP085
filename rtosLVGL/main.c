#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"

#include "st7735.h"
#include "bmp085.h"
#include "buttons.h"

#include "driv_lvgl.h"
#include "lvgl.h"

lv_obj_t *helloScreen;
lv_obj_t *helloLabel;
lv_obj_t *timeLabel;

lv_obj_t *tempScreen;
lv_obj_t *tempArc;
lv_obj_t *tempLabel;

lv_obj_t *presScreen;
lv_obj_t *presArc;
lv_obj_t *presLabel;

lv_obj_t *tempChartScreen;
lv_obj_t *tempChart;
lv_chart_series_t *tempSeries;

lv_obj_t *presChartScreen;
lv_obj_t *presChart;
lv_chart_series_t *presSeries;

TaskHandle_t blinkHandle;
void blinkTask(void *p)
{
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  int c = 0;

  while (true)
  {
    gpio_put(PICO_DEFAULT_LED_PIN, (c++) % 2);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

TaskHandle_t sensorHandle;
void sensorTask(void *p)
{

  int cnt = 0;
  BMP085_init(BMP085_STANDARD);
  float temp, pres;
  while (true)
  {
    takeLvglSemaphore();
    temp = BMP085_readTemperature();
    pres = BMP085_readPressure() / 100.0;
    if (lv_scr_act() == tempScreen)
    {
      lv_arc_set_value(tempArc, temp);
      lv_label_set_text_fmt(tempLabel, "%.1f °C", temp);
      lv_obj_align(tempLabel, LV_ALIGN_CENTER, 0, 0);
    }
    else if (lv_scr_act() == presScreen)
    {

      lv_arc_set_value(presArc, pres);
      lv_label_set_text_fmt(presLabel, "%.1f hPa", pres);
      lv_obj_align(presLabel, LV_ALIGN_CENTER, 0, 0);
    }
    else if (lv_scr_act() == helloScreen)
    {
      int s = xTaskGetTickCount() / configTICK_RATE_HZ;
      lv_label_set_text_fmt(timeLabel, "%ds", s);
      lv_obj_align(timeLabel, LV_ALIGN_CENTER, 0, 0);
    }
    else if (lv_scr_act() == tempChartScreen)
    {
      lv_chart_refresh(tempChart);
    }
    else if (lv_scr_act() == presChartScreen)
    {
      lv_chart_refresh(presChart);
    }
    if (cnt % (1000) == 0)
    {
      lv_chart_set_next_value(tempChart, tempSeries, temp);
      lv_chart_set_next_value(presChart, presSeries, pres);
    }

    cnt += 50;
    giveLvglSemaphore();
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

TaskHandle_t buttonHandle;
void buttonTask(void *p)
{
  initButtons();

  static bool pa;
  static bool pb;
  while (true)
  {
    bool ba = getButton(BTN_A);
    bool bb = getButton(BTN_B);

    if (ba && !pa)
    {
      prevScreen();
    }

    if (bb && !pb)
    {
      nextScreen();
    }

    pa = ba;
    pb = bb;
  }
  vTaskDelay(pdMS_TO_TICKS(100));
}

void createUI()
{
  tempScreen = lv_obj_create(NULL);
  presScreen = lv_obj_create(NULL);
  tempChartScreen = lv_obj_create(NULL);
  presChartScreen = lv_obj_create(NULL);
  helloScreen = lv_obj_create(NULL);

  addToCarousel(tempScreen);
  addToCarousel(presScreen);
  addToCarousel(tempChartScreen);
  addToCarousel(presChartScreen);
  addToCarousel(helloScreen);

  tempArc = lv_arc_create(tempScreen);
  lv_obj_set_size(tempArc, 100, 100);
  lv_arc_set_range(tempArc, 0, 50);
  lv_obj_align(tempArc, LV_ALIGN_CENTER, 0, 0);
  tempLabel = lv_label_create(tempArc);
  lv_obj_align(tempLabel, LV_ALIGN_CENTER, 0, 0);

  presArc = lv_arc_create(presScreen);
  lv_obj_set_size(presArc, 100, 100);
  lv_arc_set_range(presArc, 900, 1100);
  lv_obj_align(presArc, LV_ALIGN_CENTER, 0, 0);
  presLabel = lv_label_create(presArc);
  lv_obj_align(presLabel, LV_ALIGN_CENTER, 0, 0);

  tempChart = lv_chart_create(tempChartScreen);
  lv_obj_set_size(tempChart, 130, 128);
  lv_chart_set_range(tempChart, LV_CHART_AXIS_PRIMARY_Y, 15, 35);
  lv_chart_set_axis_tick(tempChart, LV_CHART_AXIS_PRIMARY_Y, 4, 2, 5, 2, true, 25);
  lv_chart_set_div_line_count(tempChart, 10, 10);
  lv_obj_align(tempChart, LV_ALIGN_RIGHT_MID, 0, 0);
  tempSeries = lv_chart_add_series(tempChart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

  presChart = lv_chart_create(presChartScreen);
  lv_obj_set_size(presChart, 120, 128);
  lv_chart_set_range(presChart, LV_CHART_AXIS_PRIMARY_Y, 900, 1000);
  lv_chart_set_axis_tick(presChart, LV_CHART_AXIS_PRIMARY_Y, 4, 2, 5, 2, true, 50);
  lv_chart_set_div_line_count(presChart, 10, 10);
  lv_obj_align(presChart, LV_ALIGN_RIGHT_MID, 0, 0);
  presSeries = lv_chart_add_series(presChart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);

  helloLabel = lv_label_create(helloScreen);
  lv_label_set_text_fmt(helloLabel, "Hello LVGL!\nRunning on\nFreeRTOS %s", tskKERNEL_VERSION_NUMBER);
  lv_obj_align(helloLabel, LV_ALIGN_TOP_MID, 0, 0);

  timeLabel = lv_label_create(helloScreen);
  lv_obj_align(timeLabel, LV_ALIGN_CENTER, 0, 0);

  goToScreen(0);
}

int main()
{
  // Initialize the LCD
  LCD_initDisplay(INITR_BLACKTAB);
  LCD_setRotation(1);

  // Create threads
  xTaskCreate(buttonTask, "buttonTask", 256, NULL, 1, &buttonHandle);
  xTaskCreate(sensorTask, "sensorTask", 256, NULL, 1, &sensorHandle);
  xTaskCreate(blinkTask, "blinkTask", 256, NULL, 1, &blinkHandle);

  // Initialize LVGL (creates LVGL thread)
  initLVGL();
  createUI();

  // Start FreeRTOS kernel
  vTaskStartScheduler();

  panic("RTOS kernel not running!"); // we shouldn't get here
}
