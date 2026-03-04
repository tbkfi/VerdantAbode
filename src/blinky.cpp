#include "blinky.hpp"


void task_create_blink(void) {
	static BLINKY::CTX ctx = { .pin = BLINKY::PIN, .delay_ms = BLINKY::DELAY_MS };
	xTaskCreate(task_blink, "LED_1", 256, (void *) &ctx, tskIDLE_PRIORITY + 1, NULL);
}

void task_blink(void *param) {
	BLINKY::CTX *ctx = (BLINKY::CTX*) param;

    gpio_init(ctx->pin);
    gpio_set_dir(ctx->pin, GPIO_OUT);

    while (true) {
        gpio_put(ctx->pin, 1); vTaskDelay(ctx->delay_ms);
        gpio_put(ctx->pin, 0); vTaskDelay(ctx->delay_ms);
		printf("[%lu] blink\n", pdTICKS_TO_MS(xTaskGetTickCount()));
    }
}
