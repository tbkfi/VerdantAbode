/* VerdantAbode
 * valve.hpp
 *
 * Tuomo Björk
*/
#pragma once

#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "FreeRTOS.h"
#include "portmacro.h"
#include "projdefs.h"
#include "task.h"
#include "event_groups.h"

#include "system.hpp"


namespace VALVE {
	constexpr bool DEBUG = false; // Print debugs?

	constexpr uint8_t PIN = 27;
	constexpr uint16_t POLL_INTERVAL_MS = 50;
	constexpr uint16_t OPEN_TIME_MS = 250;
	constexpr uint16_t COOLDOWN_MS = 1750;
	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 5;
	constexpr uint16_t STACK_DEPTH = 1024;

	void task(void* param);
	void create_task(SYSTEM::DATA* ctx);

	void open(bool open, SYSTEM::DATA* ctx);
}

