/* VerdantAbode
 * valve.cpp
 *
 * Tuomo Björk
*/
#include "valve.hpp"


void task_create_valve(SYSTEM::DATA* ctx) {
	xTaskCreate(task_valve, "VALVE", VALVE::STACK_DEPTH, (void *) &ctx, VALVE::TASK_PRIORITY, NULL);
}

void task_valve(void* param) {
	SYSTEM::DATA* ctx = (SYSTEM::DATA*) param;

	TickType_t last_ran = xTaskGetTickCount();
	TickType_t poll_time = pdMS_TO_TICKS(VALVE::POLL_INTERVAL_MS);

	gpio_init(VALVE::PIN);
	gpio_set_dir(VALVE::PIN, GPIO_OUT);

	while (true) {
		vTaskDelayUntil(&last_ran, poll_time);
		
		EventBits_t flags = xEventGroupGetBits(ctx->events);
		bool open_valve = (flags & SYSTEM::FLAG_VALVE_OPEN);

		TickType_t now_ms = pdTICKS_TO_MS(xTaskGetTickCount());
		bool cooldown_passed = (now_ms - ctx->time_valve_closed_ms >= VALVE::COOLDOWN_MS);


		if (!open_valve) {
		// Keep valve shut by default
			gpio_put(VALVE::PIN, false);
			if (VALVE::DEBUG) printf("[VALVE] (status): CLOSED.");
		}

		if (!cooldown_passed) {
		// CO2 needs time to stabilise, disallow opening again before cooldown reached
			if (VALVE::DEBUG) printf("[VALVE] (cooldown): not reached.");
			continue;
		}

		if (now_ms - ctx->time_valve_opened_ms >= VALVE::OPEN_TIME_MS) {
		// Valve should only be open for a period of time!
			xEventGroupClearBits(ctx->events, SYSTEM::FLAG_VALVE_OPEN);
			ctx->time_valve_closed_ms = now_ms;
			continue;
		}

		if (open_valve) {
		// Open
			gpio_put(VALVE::PIN, true);
			ctx->time_valve_opened_ms = now_ms;
			if (VALVE::DEBUG) printf("[VALVE] (status): OPEN.");

		}
	}
}
