// Objective: Hold CO2 level steady at defined value
#include "co2_monitor.hpp"


void task_co2_monitor(void* param) {
	SYS::STATE *ctx = (SYS::STATE*) param;

    TickType_t last_ran = xTaskGetTickCount();
    TickType_t poll_time = pdMS_TO_TICKS(CO2_MONITOR::POLL_MS);

	// Wait for initial CO2 reading.
	GMP252::MEASUREMENT co2_current;
	xQueuePeek(ctx->gmp252_queue, &co2_current, portMAX_DELAY);

	while (true) {
        vTaskDelayUntil(&last_ran, poll_time);

		// Ventilate if CO2 level is exceeded
		if (co2_current.data > ctx->co2_setpoint) {
			// 1. Open Vent

			// 2. Fan at 2s increments
		}
	}
}
