cmake_minimum_required(VERSION 3.14)

add_executable(rtosLvgl
        main.c
        driv_lvgl.c
        screenCarousel.c
        buttons.c
        )
        
target_link_libraries(rtosLvgl pico_stdlib st7735 bmp085 freertos lvgl) 

# create map/bin/hex file etc.
pico_add_extra_outputs(rtosLvgl)

