#include "pico/stdlib.h"
#include "buttons.h"

void initButtons()
{
    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_up(BTN_A);

    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_B);
}

bool getButton(uint b)
{
    return !(gpio_get(b));
}