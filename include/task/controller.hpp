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


namespace CONTROLLER {
	constexpr uint16_t POLL_MS = 500;
	constexpr uint16_t CO2_ADD_TIME_MS = 2;
	constexpr uint16_t CO2_RMV_TIME_MS = 15;
	constexpr uint16_t CO2_CRITICAL_THRESH = 2000;

	// Task settings
	constexpr uint16_t STACK_DEPTH = 1024;
	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 3; 
}

void task_controller(void *param);
void task_create_controller(SYSTEM::DATA* ctx);
