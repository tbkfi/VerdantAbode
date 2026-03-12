/* VerdantAbode
 * gmp252.cpp
 *
 * Pavel Shishkin
*/
#include <gmp252.hpp>


QueueHandle_t
GMP252::create_task
(SemaphoreHandle_t mutex_uart, std::shared_ptr<ModbusClient> rtu_client, std::shared_ptr<PicoUart> uart)
{
	static GMP252::CTX ctx;
	ctx.mutex = mutex_uart;
	ctx.rtu_client = rtu_client;
	ctx.uart = uart;
	ctx.que = xQueueCreate(GMP252::QUE_LEN, sizeof(GMP252::QUE_ELEMENT));

	// Validation
	if (ctx.que == NULL) {
		while (true) printf("[GMP252] Que is NULL!\n");
	}
	vQueueAddToRegistry(ctx.que, "GMP252");
	
	xTaskCreate(GMP252::task, "GMP252", GMP252::STACK_DEPTH, (void *) &ctx, GMP252::TASK_PRIORITY, NULL);

	return ctx.que;
}

void GMP252::task(void* param) {
	GMP252::CTX* ctx = (GMP252::CTX*) param;
	ModbusRegister gmp252 (ctx->rtu_client, GMP252::ADDRESS, GMP252::REGISTER::CO2_16);

	int16_t reading_raw = 0;
	GMP252::QUE_ELEMENT e;

	TickType_t last_ran = xTaskGetTickCount();
	const TickType_t poll_time = pdMS_TO_TICKS(GMP252::POLL_INTERVAL_MS);

	while (true) {
		vTaskDelayUntil(&last_ran, poll_time);

		// Require Mutex
		if (xSemaphoreTake(ctx->mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
			if (GMP252::DEBUG) printf("[GMP252] Mutex busy. Skipping this cycle.\n");
			continue;
		}

		// Read Measurement
		ctx->uart->flush();
		if (GMP252::DEBUG) printf("[GMP252] Attempting to read...\n");

		reading_raw = (int16_t) gmp252.read();
		xSemaphoreGive(ctx->mutex);

		e.time_ms = pdTICKS_TO_MS(xTaskGetTickCount());
		e.data = reading_raw;

		xQueueSend(ctx->que, &e, 0);
		if (GMP252::DEBUG) printf("[%lu] GMP252: %d ppm\n", e.time_ms, e.data);
	}
}
