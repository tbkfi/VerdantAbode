/* VerdantAbode
 * controller.cpp
 *
 * Tuomo Björk
*/
#include "controller.hpp"
#include "mio.hpp"
#include "system.hpp"


void CONTROLLER::create_task(SYSTEM::DATA* ctx) {
	xTaskCreate(CONTROLLER::task, "CONTROLLER",
			 CONTROLLER::STACK_DEPTH, (void*) ctx, CONTROLLER::TASK_PRIORITY, NULL);
}

void CONTROLLER::task(void* param) {
	SYSTEM::DATA* ctx = (SYSTEM::DATA*)param;
	
	TickType_t last_wake = xTaskGetTickCount();
	TickType_t interval = pdMS_TO_TICKS(CONTROLLER::POLL_MS);

	while (true) {
		vTaskDelayUntil(&last_wake, interval);

		if (ctx->val_co2 >= SYSTEM::CO2_CRITICAL) {
		// CO2 Exceeds CRITICAL
			VALVE::open(false, ctx);
			if (ctx->mio_queue != nullptr) FAN::set_speed(CONTROLLER::PR_FLUSH_CRIT, ctx->mio_queue);
		}
		else if (ctx->val_co2 > (ctx->co2_target + SYSTEM::CO2_SPAN)) {
		// CO2 above target
			VALVE::open(false, ctx);
			if (ctx->mio_queue != nullptr) FAN::set_speed(CONTROLLER::PR_FLUSH_CRIT, ctx->mio_queue);
		}
		else if (ctx->val_co2 < (ctx->co2_target - SYSTEM::CO2_SPAN)) {
		// CO2 below target
			VALVE::open(true, ctx);
			if (ctx->mio_queue != nullptr) FAN::set_speed(0, ctx->mio_queue);
		}
		else {
		// Nominal
			VALVE::open(false, ctx);
			if (ctx->mio_queue != nullptr) FAN::set_speed(0, ctx->mio_queue);
		}
	}
}
