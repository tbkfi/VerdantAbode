/* Example usage:

	static led_params lp1 = { .pin = 20, .delay = 250 };
	static led_params lp2 = { .pin = 21, .delay = 450 };
	xTaskCreate(blink_task, "LED_1", 256, (void *) &lp1, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(blink_task, "LED_2", 256, (void *) &lp2, tskIDLE_PRIORITY + 1, NULL);

*/

#pragma once
#include "pico/stdlib.h"

struct led_params{
    uint pin;
    uint delay;
};

void blink_task(void *param);
