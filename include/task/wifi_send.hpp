/* VerdantAbode
 * wifi_send.hpp
 *
 * Tuomo Björk
*/
#pragma once

#include <cstdio>

#include "FreeRTOS.h"
#include "projdefs.h"
#include "task.h"
#include "event_groups.h"

#include "pico/cyw43_arch.h"
#include "lwip/apps/http_client.h"

#include "wifi.hpp"
#include "system.hpp"


namespace WIFI_SEND {
	constexpr bool DEBUG = true; // Print debugs?

	constexpr uint16_t STACK_DEPTH = 2048;
	constexpr uint8_t TASK_PRIORITY = tskIDLE_PRIORITY + 4;
	constexpr TickType_t TASK_INTERVAL_MS = pdMS_TO_TICKS(30*1000);

	constexpr const char* API_KEY = "***REMOVED***";
	void create_task(SYSTEM::DATA* ctx);
	void task(void *param);
}
