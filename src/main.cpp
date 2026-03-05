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


int main() {
	// INIT HW
	stdio_init_all();
	init_i2c0();
	init_i2c1();

	// SYSTEM
	SYSTEM::DATA system;
	system.co2_target = 1200;
	system.mutex_i2c = xSemaphoreCreateMutex();
	
	// TASKS
	task_create_blink();
	system.input_queue = create_local_inputs();
	system.sdp610_queue = task_create_sdp610(system.mutex_i2c);

	vTaskStartScheduler();
    while(true);
}
