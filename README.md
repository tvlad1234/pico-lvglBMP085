# pico-lvglBMP085
*Thermometer demo, using LVGL and FreeRTOS on the Pi Pico* \
This demo uses a ST7735-based, 128x160 SPI LCD and a BMP085/BMP180 temperature and pressure sensor. It uses the [LVGL](https://github.com/lvgl/lvgl) library for the UI and runs on top of
FreeRTOS.\
The sensor is connected to the default I2C pins (SDA on GPIO4, SCL on GPIO5).\
For the LCD, RST is GPIO16, CS is GPIO17, DC is GPIO20, SCK is GPIO18, TX is GPIO19.\
Two buttons are used for navigation. They are connected from ground to GPIO14 and GPIO15.

