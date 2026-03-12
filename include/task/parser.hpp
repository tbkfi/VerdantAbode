/* VerdantAbode
 * parser.hpp
 *
 * Tuomo Björk
*/
#pragma once

#include "stdio.h"
#include "FreeRTOS.h"
#include "queue.h"

#include "system.hpp"

#include "local_inputs.hpp"
#include "sdp610.hpp"
#include "gmp252.hpp"
#include "hmp60.hpp"
#include <cstdint>


namespace PARSER {
	constexpr bool DEBUG = true;
	constexpr uint16_t INTERVAL_MS = 10;
	constexpr uint16_t STACK_DEPTH = 1024;
	constexpr uint8_t TASK_PRIORITY = tskIDLE_PRIORITY + 10;

	void task(void* param);
	void create_task(SYSTEM::DATA* ctx);
}
