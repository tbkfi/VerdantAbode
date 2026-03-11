/* VerdantAbode
 * action_hmp60.cpp
 *
 * Tuomo Björk
*/
#include "action_hmp60.hpp"


void action_hmp60(SYSTEM::DATA* ctx, HMP60::QUE_ELEMENT* e) {
// Action(s) taken when a CO2 READING is received.
	printf("[%lu] PARSER(hmp60): %d\n", e->time_ms, e->data);

	ctx->val_temp = e->data;
}
