/* VerdantAbode
 * main.cpp
 *
 * Tuomo Björk
*/
#include "hardware/timer.h"
extern "C" {
	uint32_t read_runtime_ctr(void) {
		return timer_hw->timerawl;
	}
}

#include "system.hpp"
#include "util.hpp"

#include "blinky.hpp"
#include "sdp610.hpp"
#include "local_inputs.hpp"
#include "parser.hpp"


int main() {
	// INIT HW
	stdio_init_all();
	init_i2c0();
	init_i2c1();

	// SYSTEM
	SYSTEM::DATA system;
	system.co2_target = 1200;
	system.events = xEventGroupCreate();
	system.mutex_i2c = xSemaphoreCreateMutex();
	system.mutex_uart = xSemaphoreCreateMutex();

	if (system.events == NULL || system.mutex_i2c == NULL || system.mutex_uart == NULL) {
		while (true) printf("[SYSTEM] Initialisation lead to NULL!\n");
	}
	
	// TASKS
	task_create_blink();
	task_create_parser(&system);
	system.input_queue = create_local_inputs();
	system.sdp610_queue = task_create_sdp610(system.mutex_i2c);

	vTaskStartScheduler();
    while(true);
}
