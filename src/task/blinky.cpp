/* VerdantAbode
 * blinky.cpp
 *
 * Tuomo Björk
*/
#include "blinky.hpp"


void task_create_blinky(UBaseType_t task_prio) {
	static BLINKY::CTX ctx = { .pin = BLINKY::PIN, .delay_ms = BLINKY::DELAY_MS };
	xTaskCreate(task_blinky, "LED_1", 256, (void *) &ctx, BLINKY::TASK_PRIO, NULL);
}

void task_blinky(void *param) {
	BLINKY::CTX *ctx = (BLINKY::CTX*) param;

    gpio_init(ctx->pin);
    gpio_set_dir(ctx->pin, GPIO_OUT);

	TickType_t last_ran = xTaskGetTickCount();
	TickType_t poll_time = pdMS_TO_TICKS(BLINKY::DELAY_MS);

    while (true) {
		vTaskDelayUntil(&last_ran, poll_time);

        gpio_put(ctx->pin, !gpio_get(ctx->pin));
		printf("[%lu] blink\n", pdTICKS_TO_MS(xTaskGetTickCount()));
    }
}
