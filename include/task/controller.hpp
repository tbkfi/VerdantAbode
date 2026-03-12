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


namespace CONTROLLER {
	constexpr uint16_t POLL_MS = 50;

	// Task settings
	constexpr uint16_t STACK_DEPTH = 1024;
	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 5; 
}

void task_controller(void *param);
void task_create_controller(SYSTEM::DATA* ctx);
