/* VerdantAbode
 * valve.cpp
 *
 * Tuomo Björk
*/
#include "valve.hpp"
#include "system.hpp"
#include "eeprom.hpp"


void VALVE::create_task(SYSTEM::DATA* ctx) {
	xTaskCreate(VALVE::task, "VALVE", VALVE::STACK_DEPTH, (void *) ctx, VALVE::TASK_PRIORITY, NULL);
}


void VALVE::task(void* param) {
	SYSTEM::DATA* ctx = (SYSTEM::DATA*) param;

	TickType_t last_ran = xTaskGetTickCount();
	TickType_t poll_time = pdMS_TO_TICKS(VALVE::POLL_INTERVAL_MS);

	gpio_init(VALVE::PIN);
	gpio_set_dir(VALVE::PIN, GPIO_OUT);
	gpio_put(VALVE::PIN, false);

	ctx->time_valve_closed_ms = pdTICKS_TO_MS(xTaskGetTickCount());

	while (true) {
		vTaskDelayUntil(&last_ran, poll_time);
		
		EventBits_t flags = xEventGroupGetBits(ctx->events);
		bool should_open = (flags & SYSTEM::FLAG_VALVE_OPEN);
		
		TickType_t now_ms = pdTICKS_TO_MS(xTaskGetTickCount());
		bool is_open = gpio_get_out_level(VALVE::PIN);

		if (!is_open) {
			if (should_open) {
			// Flag tells us we should open
				bool cooldown_passed = (now_ms - ctx->time_valve_closed_ms >= VALVE::COOLDOWN_MS);
				
				if (cooldown_passed) {
				// Permit opening if cooldown is elapsed
					gpio_put(VALVE::PIN, true);
					ctx->time_valve_opened_ms = now_ms;
					if (VALVE::DEBUG) printf("[VALVE] (status): OPEN.\n");
				} else {
					if (VALVE::DEBUG) printf("[VALVE] (cooldown): waiting to open...\n");
				}
			}
		} else {
		// Flag tells us we should close
			bool open_time_passed = (now_ms - ctx->time_valve_opened_ms >= VALVE::OPEN_TIME_MS);
			
			if (open_time_passed || !should_open) {
			// Valve should be open only for the defined period on each signal!
				gpio_put(VALVE::PIN, false);

				// Enforce cooldown before next open is permitted
				ctx->time_valve_closed_ms = now_ms;
				EEPROM::save(ctx);
				
				if (should_open) xEventGroupClearBits(ctx->events, SYSTEM::FLAG_VALVE_OPEN);
				if (VALVE::DEBUG) printf("[VALVE] (status): CLOSED. Cooldown started.\n");
			}
		}
	}
}

void VALVE::open(bool open, SYSTEM::DATA* ctx) {
	if (VALVE::DEBUG) printf("[VALVE] (cmd): %s.\n", open ? "OPEN" : "CLOSE");
	if (open) xEventGroupSetBits(ctx->events, SYSTEM::FLAG_VALVE_OPEN);
	else xEventGroupClearBits(ctx->events, SYSTEM::FLAG_VALVE_OPEN);
}
