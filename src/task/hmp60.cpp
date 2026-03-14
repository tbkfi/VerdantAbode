/* VerdantAbode
 * hmp60.cpp
 *
 * Tuomo Björk
*/
#include <hmp60.hpp>


QueueHandle_t HMP60::create_task(SemaphoreHandle_t mutex_uart,
	std::shared_ptr<ModbusClient> rtu_client, std::shared_ptr<PicoUart> uart) {
	static HMP60::CTX ctx;
	ctx.rtu_client = rtu_client;
	ctx.mutex = mutex_uart;
	ctx.uart = uart;
	ctx.que = xQueueCreate(HMP60::QUE_LEN, sizeof(HMP60::QUE_ELEMENT));

	// Validation
	if (ctx.que == NULL) {
		while (true) printf("[HMP60] Que is NULL!\n");
	}
	vQueueAddToRegistry(ctx.que, "HMP60");
	
	xTaskCreate(HMP60::task, "HMP60", HMP60::STACK_DEPTH, (void *) &ctx, HMP60::TASK_PRIORITY, NULL);
	return ctx.que;
}

void HMP60::task(void* param) {
	HMP60::CTX* ctx = (HMP60::CTX*) param;

	// Rh
	ModbusRegister rh_reg_lsw(ctx->rtu_client, HMP60::ADDRESS, HMP60::REGISTER::Rh);
	ModbusRegister rh_reg_msw(ctx->rtu_client, HMP60::ADDRESS, HMP60::REGISTER::Rh + 1);

	// T
	ModbusRegister T_reg_lsw(ctx->rtu_client, HMP60::ADDRESS, HMP60::REGISTER::T);
	ModbusRegister T_reg_msw(ctx->rtu_client, HMP60::ADDRESS, HMP60::REGISTER::T + 1);

	TickType_t last_ran = xTaskGetTickCount();
	const TickType_t poll_time = pdMS_TO_TICKS(HMP60::POLL_INTERVAL_MS);

	while (true) {
		vTaskDelayUntil(&last_ran, poll_time);

		// Require Mutex
		if (xSemaphoreTake(ctx->mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
			if (HMP60::DEBUG) printf("[HMP60] Mutex unavailable.\n");
			continue;
		}
		ctx->uart->flush();

		HMP60::QUE_ELEMENT e;
		e.time_ms = pdTICKS_TO_MS(xTaskGetTickCount());

		uint16_t word_lsw = 0;
		uint16_t word_msw = 0;
		uint32_t combined = 0;

		// Relative Humidity
		if (HMP60::DEBUG) printf("[HMP60] (read): attempt to get Rh ...\n");
		word_lsw = rh_reg_lsw.read();
		word_msw = rh_reg_msw.read();

		combined = ((uint32_t)word_msw << 16) | word_lsw;
		memcpy(&e.data_rh, &combined , sizeof(float));

		word_lsw = 0;
		word_msw = 0;
		combined = 0;

		// Temperature
		if (HMP60::DEBUG) printf("[HMP60] (read): attempt to get T...\n");
		e.time_ms = pdTICKS_TO_MS(xTaskGetTickCount());
		word_lsw = T_reg_lsw.read();
		word_msw = T_reg_msw.read();
		xSemaphoreGive(ctx->mutex);

		combined = ((uint32_t)word_msw << 16) | word_lsw;
		memcpy(&e.data_t, &combined , sizeof(float));

		xQueueSend(ctx->que, &e, 0);
		if (HMP60::DEBUG) printf("[%lu] HMP60: %.2f C, %f Rh\n", e.time_ms, e.data_t, e.data_rh);
	}
}
