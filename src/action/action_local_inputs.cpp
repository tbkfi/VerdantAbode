/* VerdantAbode
 * action_local_input.cpp
 *
 * Tuomo Björk
*/
#include "action_local_inputs.hpp"
#include "system.hpp"

#include "mio.hpp"
#include "valve.hpp"
#include "eeprom.hpp"


void action_local_input_regular(SYSTEM::DATA* ctx, LOCAL_INPUTS::QUE_ELEMENT* e) {
// Action(s) taken when a LOCAL INPUT is received.
	printf("[%lu] PARSER(local_input): %u\n", e->time_ms, e->pin);

	switch (e->pin) {
		case LOCAL_INPUTS::ROTA_PIN:
		// Increment CO2 target
			ctx->co2_target += SYSTEM::CO2_INCR;
			if (ctx->co2_target > SYSTEM::CO2_CEIL) ctx->co2_target = SYSTEM::CO2_CEIL;
			printf("[%lu] PARSER(co2_target++): %d\n", e->time_ms, ctx->co2_target);
			break;
		case LOCAL_INPUTS::ROTB_PIN:
		// Decrement CO2 target
			ctx->co2_target -= SYSTEM::CO2_INCR;
			if (ctx->co2_target < SYSTEM::CO2_FLOOR) ctx->co2_target = SYSTEM::CO2_FLOOR;
			printf("[%lu] PARSER(co2_target--): %d\n", e->time_ms, ctx->co2_target);
			break;
		case LOCAL_INPUTS::BTN1_PIN:
		// Unassigned
			EEPROM::save(ctx, ctx->mutex_i2c);
			break;
		case LOCAL_INPUTS::BTN2_PIN:
		// Unassigned
			break;
		case LOCAL_INPUTS::BTN3_PIN:
		{ // Toggle MAIN | SETUP state
			EventBits_t events = xEventGroupGetBits(ctx->events);
			if (events & SYSTEM::FLAG_WIFI_SETUP) {
				xEventGroupClearBits(ctx->events, SYSTEM::FLAG_WIFI_SETUP);
			} else {
				xEventGroupSetBits(ctx->events, SYSTEM::FLAG_WIFI_SETUP);
			}
			break;
		}
		default:
		// Undefined
			break;
	}
}

void action_local_input_wifi_setup(SYSTEM::DATA* ctx, LOCAL_INPUTS::QUE_ELEMENT* e) {
}
