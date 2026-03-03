// System
#include <cstdint>
#include <hardware/gpio.h>
#include <hardware/structs/io_bank0.h>
#include <pico/time.h>
#include <stdio.h>
#include <stdlib.h>

// Pico
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// FreeRTOS
#include "FreeRTOS.h"
#include "portmacro.h"
#include "projdefs.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// Project
#include "system.hpp"
#include "namespace/pin.hpp"
#include "namespace/config.hpp"

#include "blinky.hpp"
#include "sdp610.hpp"
#include "gmp252.hpp"

// Libs
#include "nanomodbus.h"


#include "hardware/timer.h"
extern "C" {
	uint32_t read_runtime_ctr(void) {
		return timer_hw->timerawl;
	}
}

enum class InputAction {
    PRESS,
    LEFT,
    RIGHT
};

struct event_input {
    InputAction action;
    uint32_t timestamp;
};

static QueueHandle_t inputs = xQueueCreate(16, sizeof(event_input)); // ISR must have access!


int main() {
	stdio_init_all();

	init_i2c();

	static SYS::STATE sys = {
		.co2_setpoint  = 1200,
		.semaphore_i2c = xSemaphoreCreateBinary(),
		.sdp610_queue  = xQueueCreate(8, sizeof(int16_t)),
		.gmp252_queue  = xQueueCreate(8, sizeof(uint32_t)),
	};

	// Blinky (sanity check)
	static led_params lp1 = { .pin = 20, .delay = 250 };
	static led_params lp2 = { .pin = 21, .delay = 450 };
	xTaskCreate(blink_task, "LED_1", 256, (void *) &lp1, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(blink_task, "LED_2", 256, (void *) &lp2, tskIDLE_PRIORITY + 1, NULL);

	// Inputs
	init_inputs();

	// SDP610
	xTaskCreate(task_sdp610, "SDP610", 256, (void *) &sys, tskIDLE_PRIORITY + 2, NULL);
	// GMP252
	
	// HMP60
	
	// MIO


    vTaskStartScheduler();
    while(true);
}
