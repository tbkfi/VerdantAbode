/* VerdantAbode
 * controller.hpp
 *
 * Tuomo Björk
*/
#pragma once

#include "stdint.h"

#include "FreeRTOS.h"
#include "task.h"

#include "system.hpp"
#include "mio.hpp"
#include "valve.hpp"


namespace CONTROLLER {
	constexpr uint16_t POLL_MS = 50;

	constexpr uint16_t PR_FLUSH_CRIT = 100;  // Critical flush speed in %
	constexpr uint16_t PR_FLUSH_NORM = 60;   // Normal flush speed in %

	// Task settings
	constexpr uint16_t STACK_DEPTH = 1024;
	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 5; 

	void task(void *param);
	void create_task(SYSTEM::DATA* ctx);
}

