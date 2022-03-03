add_subdirectory(pico-bmp085Driv)
add_subdirectory(freertos)
add_subdirectory(lvgl)
add_subdirectory(pico-displayDrivs)

target_include_directories(lvgl PUBLIC
    ./
)