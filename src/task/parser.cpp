/* VerdantAbode
 * parser.cpp
 *
 * Tuomo Björk
*/
#include "parser.hpp"

#include "system.hpp"

#include "action_local_inputs.hpp"
//#include "action_wifi_setup.hpp"
#include "action_sdp610.hpp"
#include "action_gmp252.hpp"


void task_create_parser(SYSTEM::DATA* ctx) {
	xTaskCreate(task_parser, "DEBUG", PARSER::STACK_DEPTH, (void*)ctx, PARSER::TASK_PRIORITY, NULL);
}

void task_parser(void* param) {
	SYSTEM::DATA* ctx = (SYSTEM::DATA*) param;

	// Elements
	LOCAL_INPUTS::QUE_ELEMENT e_local_input;
	SDP610::QUE_ELEMENT e_sdp610;
	GMP252::QUE_ELEMENT e_gmp252;
	
	TickType_t last_ran = xTaskGetTickCount();
	TickType_t interval = pdMS_TO_TICKS(PARSER::INTERVAL_MS);

	uint32_t flags = 0;
	while (true) {
		vTaskDelayUntil(&last_ran, interval);
		flags = xEventGroupGetBits(ctx->events);

		// Local inputs
		if (ctx->input_queue != nullptr && xQueueReceive(ctx->input_queue, &e_local_input, 0) == pdTRUE) {
			//if (flags & SYSTEM::FLAG_WIFI_SETUP) action_wifi_setup(ctx, &e_local_input);
			//else action_local_input_regular(ctx, &e_local_input);
			action_local_input_regular(ctx, &e_local_input);
		}
		// SDP610 for Pa
		if (ctx->sdp610_queue != nullptr && xQueueReceive(ctx->sdp610_queue, &e_sdp610, 0) == pdTRUE) {
			action_sdp610(ctx, &e_sdp610);
		}
		// GMP252 for CO2
		if (ctx->gmp252_queue != nullptr && xQueueReceive(ctx->gmp252_queue, &e_gmp252, 0) == pdTRUE) {
			action_gmp252(ctx, &e_gmp252);
		}
	}
}
