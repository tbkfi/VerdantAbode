//#include "system.hpp"
//#include "pin.hpp"
//#include "config.hpp"

#include "system.hpp"

// [TASKS]
#include "blinky.hpp"
#include "sdp610.hpp"
//#include "gmp252.hpp"

//#include "pin.hpp"
//#include "config.hpp"

//static QueueHandle_t inputs = xQueueCreate(16, sizeof(event_input)); // ISR must have access!

 extern "C" {
    uint32_t read_runtime_ctr(void) {
        return timer_hw->timerawl;
    }
} 

int main() {
	// [INIT HW]
	stdio_init_all();

	// [SYSTEM]
	SYSTEM::DATA system;
	system.co2_setpoint = 1200;
	
	// [TASKS]
	task_create_blink();
	//system.sdp610_queue = task_create_sdp610(&system);


    vTaskStartScheduler();





	
	// Initialisations
	//init_inputs();
	//init_i2c();

/*
	static SYSTEM::DATA sys = {
		.co2_setpoint  = 1200,
		.semaphore_i2c = xSemaphoreCreateBinary(),
		.sdp610_queue  = xQueueCreate(8, sizeof(int16_t)),
		.gmp252_queue  = xQueueCreate(8, sizeof(uint32_t)),
	};
*/


	// SDP610
	//xTaskCreate(task_sdp610, "SDP610", 256, (void *) &sys, tskIDLE_PRIORITY + 2, NULL);
	// GMP252
	
	// HMP60
	
	// MIO


    while(true);
}
