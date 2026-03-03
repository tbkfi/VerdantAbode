#include "blinky.hpp"

void task_blink(void *param) {
    led_params *lpr = (led_params *) param;
    const uint led_pin = lpr->pin;
    const uint delay = lpr->delay;

    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    while (true) {
        gpio_put(led_pin, 1); vTaskDelay(delay);
        gpio_put(led_pin, 0); vTaskDelay(delay);
		printf("[%lu] blink\n", to_ms_since_boot(get_absolute_time()));
    }
}


void task_create_blink(void) {
	// Blinky (sanity check)
	static led_params lp1 = { .pin = 20, .delay = 250 };
	static led_params lp2 = { .pin = 21, .delay = 450 };
	xTaskCreate(task_blink, "LED_1", 256, (void *) &lp1, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(task_blink, "LED_2", 256, (void *) &lp2, tskIDLE_PRIORITY + 1, NULL);
}
