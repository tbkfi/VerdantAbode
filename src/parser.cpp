/* VerdantAbode
 * parser.cpp
 *
 * Tuomo Björk
*/
#include "parser.hpp"


void task_create_parser(SYSTEM::DATA* ctx) {
	xTaskCreate(task_parser, "DEBUG", PARSER::STACK_DEPTH, (void*)ctx, PARSER::TASK_PRIORITY, NULL);
}

void task_parser(void* param) {
	SYSTEM::DATA* ctx = (SYSTEM::DATA*) param;

	// Elements
	LOCAL_INPUTS::QUE_ELEMENT e_local_input;
	SDP610::QUE_ELEMENT e_sdp610;
	GMP252::QUE_ELEMENT e_gmp252;
	
	while (true) {
		// Local inputs
		if (ctx->input_queue != NULL && xQueueReceive(ctx->input_queue, &e_local_input, 0) == pdTRUE)
			action_local_input(ctx, &e_local_input);
		// SDP610 for Pa
		if (ctx->sdp610_queue != NULL && xQueueReceive(ctx->sdp610_queue, &e_sdp610, 0) == pdTRUE)
			action_sdp610(ctx, &e_sdp610);
		// GMP252 for CO2
		if (ctx->gmp252_queue != NULL && xQueueReceive(ctx->gmp252_queue, &e_gmp252, 0) == pdTRUE)
			action_gmp252(ctx, &e_gmp252);
	}
}


void action_local_input(SYSTEM::DATA* ctx, LOCAL_INPUTS::QUE_ELEMENT* e) {
// Action(s) taken when a LOCAL INPUT is received.
	printf("[%lu] PARSER(local_input): %u\n", e->time_ms, e->pin);
	switch (e->pin) {
		case LOCAL_INPUTS::ROTA_PIN:
		// Increment CO2 target
			ctx->co2_target += SYSTEM::CO2_INCR;
			if (ctx->co2_target > SYSTEM::CO2_CEIL) ctx->co2_target = SYSTEM::CO2_CEIL;
			printf("[%lu] PARSER(co2_target++): %lu\n", e->time_ms, ctx->co2_target);
			break;
		case LOCAL_INPUTS::ROTB_PIN:
		// Decrement CO2 target
			ctx->co2_target -= SYSTEM::CO2_INCR;
			if (ctx->co2_target < SYSTEM::CO2_FLOOR) ctx->co2_target = SYSTEM::CO2_FLOOR;
			printf("[%lu] PARSER(co2_target--): %lu\n", e->time_ms, ctx->co2_target);
			break;
		case LOCAL_INPUTS::BTN1_PIN:
		// Unassigned
			break;
		case LOCAL_INPUTS::BTN2_PIN:
		// Unassigned
			break;
		case LOCAL_INPUTS::BTN3_PIN:
		// Unassigned
			break;
		default:
		// Undefined
			break;
	}
}

void action_sdp610(SYSTEM::DATA* ctx, SDP610::QUE_ELEMENT* e) {
// Action(s) taken when a PRESSURE READING is received.
}

void action_gmp252(SYSTEM::DATA* ctx, GMP252::QUE_ELEMENT* e) {
// Action(s) taken when a CO2 READING is received.
	printf("[%lu] PARSER(sdp610): %d\n", e->time_ms, e->data);

	// CO2 exceeds threshold!
	if (e->data > ctx->co2_target) {
		xEventGroupSetBits(ctx->events, SYSTEM::FLAG_CO2_HIGH);
	}

	// CO2 within threshold!
	if (xEventGroupGetBits(ctx->events) & SYSTEM::FLAG_CO2_HIGH && e->data <= ctx->co2_target) {
		xEventGroupClearBits(ctx->events, SYSTEM::FLAG_CO2_HIGH);
	}
}
