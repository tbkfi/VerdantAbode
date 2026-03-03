#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

#include "blinky.hpp"
#include "util.hpp"


void blink_task(void *param) {
    led_params *lpr = (led_params *) param;
    const uint led_pin = lpr->pin;
    const uint delay = lpr->delay;

    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    while (true) {
        gpio_put(led_pin, 1); vTaskDelay(delay);
        gpio_put(led_pin, 0); vTaskDelay(delay);
		printf("[%u] blink\n", uptime_ms());
    }
}
