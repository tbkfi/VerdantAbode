/* VerdantAbode
 * blinky.cpp
 *
 * Tuomo Björk
*/
#include "blinky.hpp"


void BLINKY::create_task(void) {
	static BLINKY::CTX ctx = {
		.pin = BLINKY::PIN,
		.delay_ms = BLINKY::DELAY_MS
	};
	xTaskCreate(BLINKY::task, "BLINKY", 256, (void *) &ctx, BLINKY::TASK_PRIO, NULL);
}

void BLINKY::task(void *param) {
	BLINKY::CTX *ctx = (BLINKY::CTX*) param;

    gpio_init(ctx->pin);
    gpio_set_dir(ctx->pin, GPIO_OUT);

	TickType_t last_ran = xTaskGetTickCount();
	TickType_t interval1 = pdMS_TO_TICKS(BLINKY::DELAY_MS);
	TickType_t interval2 = pdMS_TO_TICKS(BLINKY::DELAY_MS/10);

    while (true) {
		vTaskDelayUntil(&last_ran, interval1);
        gpio_put(ctx->pin, !gpio_get(ctx->pin));
		if (BLINKY::DEBUG) printf("[%lu] blink (1/2)\n", pdTICKS_TO_MS(xTaskGetTickCount()));
		vTaskDelayUntil(&last_ran, interval2);
        gpio_put(ctx->pin, !gpio_get(ctx->pin));
		if (BLINKY::DEBUG) printf("[%lu] blink (2/2)\n", pdTICKS_TO_MS(xTaskGetTickCount()));
    }
}
