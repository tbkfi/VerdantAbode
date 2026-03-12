/* VerdantAbode
 * action_gmp252.cpp
 *
 * Tuomo Björk
*/
#include "action_gmp252.hpp"


void action_gmp252(SYSTEM::DATA* ctx, GMP252::QUE_ELEMENT* e) {
// Action(s) taken when a CO2 READING is received.
	printf("[%lu] PARSER(gmp252): %d ppm\n", e->time_ms, e->data);

	if (e->data != 0) ctx->val_co2 = e->data;

	// CO2 exceeds threshold!
	if (e->data > ctx->co2_target) {
		xEventGroupSetBits(ctx->events, SYSTEM::FLAG_CO2_HIGH);
	}

	// CO2 within threshold!
	if (xEventGroupGetBits(ctx->events) & SYSTEM::FLAG_CO2_HIGH && e->data <= ctx->co2_target) {
		xEventGroupClearBits(ctx->events, SYSTEM::FLAG_CO2_HIGH);
	}
}
