/* VerdantAbode
 * controller.cpp
 *
 * Tuomo Björk
*/
#include "controller.hpp"
#include "system.hpp"


// temp funcs
void valve(bool open) {}


void task_create_controller(SYSTEM::DATA* ctx) {
	xTaskCreate(task_controller, "CONTROLLER",
			 CONTROLLER::STACK_DEPTH, (void*) ctx, CONTROLLER::TASK_PRIORITY, NULL);
}

void task_controller(void* param) {
	SYSTEM::DATA* ctx = (SYSTEM::DATA*)param;
	
	// Timing
	TickType_t last_wake = xTaskGetTickCount();
	TickType_t interval = pdMS_TO_TICKS(CONTROLLER::POLL_MS);
	TickType_t valve_timer = xTaskGetTickCount();

	// const TickType_t CO2_ADD_TIME = pdMS_TO_TICKS(CONTROLLER::CO2_ADD_TIME_MS);
	// const TickType_t CO2_RMV_TIME = pdMS_TO_TICKS(CONTROLLER::CO2_RMV_TIME_MS);

	while (true) {
		vTaskDelayUntil(&last_wake, interval);

		if (ctx->val_co2 <= ctx->val_co2 - SYSTEM::CO2_SPAN ||
			ctx->val_co2 >= ctx->val_co2 + SYSTEM::CO2_SPAN) {
		// CO2 is within tolerances
		}
		else if (ctx->val_co2 >= SYSTEM::CO2_CRITICAL) {
		// CO2 Exceeds CRITICAL target
			valve(false);
			fan_set_speed(100, ctx->mio_queue);
		}
		else if (ctx->val_co2 > ctx->co2_target) {
		// CO2 Exceeds NORMAL target
			valve(false);
			fan_set_speed(25, ctx->mio_queue);
		}
		else if (ctx->val_co2 < ctx->co2_target) {
		// CO2 below target
			valve(true);
		}
	}
}
