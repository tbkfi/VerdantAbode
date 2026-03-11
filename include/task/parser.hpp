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
#include <cstdint>


namespace PARSER {
	constexpr bool DEBUG = true;
	constexpr uint16_t INTERVAL_MS = 25;
	constexpr uint16_t STACK_DEPTH = 1024;
	constexpr uint8_t TASK_PRIORITY = tskIDLE_PRIORITY + 6;
}

void task_parser(void* param);
void task_create_parser(SYSTEM::DATA* ctx);

void action_local_input(SYSTEM::DATA* ctx, LOCAL_INPUTS::QUE_ELEMENT* e);
void action_sdp610(SYSTEM::DATA* ctx, SDP610::QUE_ELEMENT* e);
void action_gmp252(SYSTEM::DATA* ctx, GMP252::QUE_ELEMENT* e);
