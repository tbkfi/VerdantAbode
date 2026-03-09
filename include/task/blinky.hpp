/* VerdantAbode
 * blinky.hpp
 *
 * Tuomo Björk
*/
#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

namespace BLINKY {
	constexpr uint8_t PIN = 20;
	constexpr uint16_t DELAY_MS = 1250;

	struct CTX {
		uint8_t pin;
		uint16_t delay_ms;
	};

}

void task_blink(void *param);
void task_create_blink(void);
