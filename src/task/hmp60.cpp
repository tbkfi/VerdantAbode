/* VerdantAbode
 * hmp60.cpp
 *
 * Tuomo Björk
*/
#include <hmp60.hpp>


QueueHandle_t task_create_hmp60 (SemaphoreHandle_t mutex_uart,
		std::shared_ptr<ModbusClient> rtu_client) {

	static HMP60::CTX ctx;
	ctx.mutex = mutex_uart;
	ctx.rtu_client = rtu_client;
	ctx.que = xQueueCreate(HMP60::QUE_LEN, sizeof(HMP60::QUE_ELEMENT));

	// Validation
	if (ctx.que == NULL) {
		while (true) printf("[HMP60] Que is NULL!\n");
	}
	vQueueAddToRegistry(ctx.que, "HMP60");
	
	xTaskCreate(task_hmp60, "HMP60", HMP60::STACK_DEPTH, (void *) &ctx, HMP60::TASK_PRIORITY, NULL);
	return ctx.que;
}

void task_hmp60(void* param) {
	HMP60::CTX* ctx = (HMP60::CTX*) param;
	ModbusRegister hmp60_t (ctx->rtu_client, HMP60::ADDRESS, HMP60::REGISTER::T);

	int16_t reading_raw = 0;
	HMP60::QUE_ELEMENT e;

	TickType_t last_ran = xTaskGetTickCount();
	const TickType_t poll_time = pdMS_TO_TICKS(HMP60::POLL_INTERVAL_MS);

	while (true) {
		vTaskDelayUntil(&last_ran, poll_time);

		// Require Mutex
		if (xSemaphoreTake(ctx->mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
			if (HMP60::DEBUG) printf("[HMP60] Mutex unavailable.\n");
			continue;
		}

		// Read Measurement
		if (HMP60::DEBUG) printf("[HMP60] Attempting to read...\n");

		reading_raw = (int16_t) hmp60_t.read();
		xSemaphoreGive(ctx->mutex);

		e.time_ms = pdTICKS_TO_MS(xTaskGetTickCount());
		e.data = reading_raw;

		xQueueSend(ctx->que, &e, 0);
		if (HMP60::DEBUG) printf("[%lu] HMP60: %d ppm\n", e.time_ms, e.data);
	}
}
