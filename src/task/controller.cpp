/* VerdantAbode
 * controller.cpp
 *
 * Tuomo Björk
*/
#include "controller.hpp"
#include "system.hpp"


void task_create_controller(SYSTEM::DATA* ctx) {
	xTaskCreate(task_controller, "CONTROLLER",
			 CONTROLLER::STACK_DEPTH, (void*) ctx, CONTROLLER::TASK_PRIORITY, NULL);
}

void task_controller(void* param) {
	SYSTEM::DATA* ctx = (SYSTEM::DATA*)param;
	
	TickType_t last_wake = xTaskGetTickCount();
	TickType_t interval = pdMS_TO_TICKS(CONTROLLER::POLL_MS);

	while (true) {
		vTaskDelayUntil(&last_wake, interval);

		if (ctx->val_co2 >= SYSTEM::CO2_CRITICAL) {
		// CO2 Exceeds CRITICAL
			valve_open(false, ctx);
			fan_set_speed(100, ctx->mio_queue);
		}
		else if (ctx->val_co2 > (ctx->co2_target + SYSTEM::CO2_SPAN)) {
		// CO2 above target
			valve_open(false, ctx);
			fan_set_speed(80, ctx->mio_queue);
		}
		else if (ctx->val_co2 < (ctx->co2_target - SYSTEM::CO2_SPAN)) {
		// CO2 below target
			valve_open(true, ctx);
			fan_set_speed(0, ctx->mio_queue);
		}
		else {
		// Nominal
			valve_open(false, ctx);
			fan_set_speed(0, ctx->mio_queue);
		}
	}
}
