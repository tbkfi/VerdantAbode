/* VerdantAbode
 * hmp60.cpp
 *
 * Tuomo Björk
*/
#include <hmp60.hpp>


QueueHandle_t HMP60::create_task(SemaphoreHandle_t mutex_uart,
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
	
	xTaskCreate(HMP60::task, "HMP60", HMP60::STACK_DEPTH, (void *) &ctx, HMP60::TASK_PRIORITY, NULL);
	return ctx.que;
}

QueueHandle_t HMP60::create_task_rh
(SemaphoreHandle_t mutex_uart, std::shared_ptr<ModbusClient> rtu_client)
{

	static HMP60::CTX ctx;
	ctx.mutex = mutex_uart;
	ctx.rtu_client = rtu_client;
	ctx.que = xQueueCreate(HMP60::QUE_LEN, sizeof(HMP60::QUE_ELEMENT));

	// Validation
	if (ctx.que == NULL) {
		while (true) printf("[HMP60] Que(RH) is NULL!\n");
	}
	vQueueAddToRegistry(ctx.que, "HMP60_RH");
	
	xTaskCreate(HMP60::task, "HMP60_RH", HMP60::STACK_DEPTH, (void *) &ctx, HMP60::TASK_PRIORITY, NULL);
	return ctx.que;
}

void HMP60::task(void* param) {
	HMP60::CTX* ctx = (HMP60::CTX*) param;
	HMP60::QUE_ELEMENT e;

	ModbusRegister reg_lsw(ctx->rtu_client, HMP60::ADDRESS, HMP60::REGISTER::T);
	ModbusRegister reg_msw(ctx->rtu_client, HMP60::ADDRESS, HMP60::REGISTER::T + 1);

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
		e.time_ms = pdTICKS_TO_MS(xTaskGetTickCount());
		uint16_t word_lsw = reg_lsw.read();
		uint16_t word_msw = reg_msw.read();
		xSemaphoreGive(ctx->mutex);

		// LSW+MSW
		uint32_t lsw_msw = ( (uint32_t)word_msw << 16) | word_lsw;

		// Reading is 'float'
		float reading;
		memcpy(&reading, &lsw_msw, sizeof(float));

		// Set element
		e.time_ms = pdTICKS_TO_MS(xTaskGetTickCount());
		e.data = reading;

		xQueueSend(ctx->que, &e, 0);
		if (HMP60::DEBUG) printf("[%lu] HMP60(T*): %.2f C\n", e.time_ms, e.data);
	}
}

void HMP60::task_rh(void* param) {
	HMP60::CTX* ctx = (HMP60::CTX*) param;
	HMP60::QUE_ELEMENT e;

	ModbusRegister reg_lsw(ctx->rtu_client, HMP60::ADDRESS, HMP60::REGISTER::RH);
	ModbusRegister reg_msw(ctx->rtu_client, HMP60::ADDRESS, HMP60::REGISTER::RH + 1);

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
		e.time_ms = pdTICKS_TO_MS(xTaskGetTickCount());
		uint16_t word_lsw = reg_lsw.read();
		uint16_t word_msw = reg_msw.read();
		xSemaphoreGive(ctx->mutex);

		// LSW+MSW
		uint32_t lsw_msw = ( (uint32_t)word_msw << 16) | word_lsw;

		// Reading is 'float'
		float reading;
		memcpy(&reading, &lsw_msw, sizeof(float));

		// Set element
		e.time_ms = pdTICKS_TO_MS(xTaskGetTickCount());
		e.data = reading;

		xQueueSend(ctx->que_rh, &e, 0);
		if (HMP60::DEBUG) printf("[%lu] HMP60(RH): %.2f C\n", e.time_ms, e.data);
	}
}
