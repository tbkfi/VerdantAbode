/* VerdantAbode
 * action_hmp60.cpp
 *
 * Tuomo Björk
*/
#include "action_hmp60.hpp"


void action_hmp60(SYSTEM::DATA* ctx, HMP60::QUE_ELEMENT* e) {
// Action(s) taken when a CO2 READING is received.
	printf("[%lu] PARSER(hmp60): %.2f C, %.2f Rh\n", e->time_ms, e->data_t, e->data_rh);

	if (e->data_t != 0) ctx->val_temp = e->data_t;
	if (e->data_rh != 0) ctx->val_rh = e->data_rh;
}
void action_hmp60_rh(SYSTEM::DATA* ctx, HMP60::QUE_ELEMENT* e) {
// Action(s) taken when a Rel. Humidity READING is received.
	printf("[%lu] PARSER(hmp60): %.1f T\n", e->time_ms, e->data);

	if (e->data != 0) ctx->val_rh = e->data;
    
}
