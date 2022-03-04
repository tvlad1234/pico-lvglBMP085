# pico-lvglBMP085
*Thermometer demo, using LVGL and FreeRTOS on the Pi Pico* \
This demo uses a ST7735-based, 128x160 SPI LCD and a BMP085/BMP180 temperature and pressure sensor. It uses the [LVGL](https://github.com/lvgl/lvgl) library for the UI and runs on top of
FreeRTOS.
## Wiring
The sensor is connected to the default I2C pins (SDA on GPIO4, SCL on GPIO5).\
For the LCD, RST is GPIO16, CS is GPIO17, DC is GPIO20, SCK is GPIO18, TX is GPIO19.\
Two buttons are used for navigation. They are connected from ground to GPIO14 and GPIO15.
## Pictures

![20220303_173324](https://user-images.githubusercontent.com/60291077/156806244-320ea36b-bc23-41cc-a16e-c5591ac33864.jpg)
![20220303_173350](https://user-images.githubusercontent.com/60291077/156806263-fb794fcf-18ea-4f8f-87ec-15a72e53eb88.jpg)
![20220303_173422](https://user-images.githubusercontent.com/60291077/156806273-f7590cd3-d7d0-489b-b504-fe3b314d62d6.jpg)
![20220303_173436](https://user-images.githubusercontent.com/60291077/156806278-81aec91a-8e9b-4a6c-bd70-6fc354547fa0.jpg)
![20220303_173508](https://user-images.githubusercontent.com/60291077/156806286-ccb663ef-d2de-40be-85b0-fc2477e25b05.jpg)
