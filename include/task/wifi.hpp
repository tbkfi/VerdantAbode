/* VerdantAbode
 * wifi.hpp
 *
 * Tuomo Björk
*/
#pragma once

#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "portmacro.h"
#include "projdefs.h"
#include "task.h"
#include "event_groups.h"

#include "system.hpp"


namespace WIFI {
	constexpr bool DEBUG = true; // Print debugs?
	constexpr uint8_t RC_ATTEMPTS = 3;
	constexpr uint16_t CONN_TIMEOUT_MS = 30*1000;
	
	constexpr const char* SSID = "SSID";
	constexpr const char* PASS = "PASSWORD";

	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 4;
	constexpr uint16_t STACK_DEPTH = 2048;
	constexpr TickType_t TASK_INTERVAL = pdMS_TO_TICKS(20*1000);

	void create_task(SYSTEM::DATA* ctx);
	void task(void *params);
}
