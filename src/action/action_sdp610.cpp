/* VerdantAbode
 * action_sdp610.cpp
 *
 * Tuomo Björk
*/
#include "action_sdp610.hpp"


void action_sdp610(SYSTEM::DATA* ctx, SDP610::QUE_ELEMENT* e) {
// Action(s) taken when a PRESSURE READING is received.
	printf("[%lu] PARSER(sdp610): %d\n", e->time_ms, e->data);

	ctx->val_pa = e->data;
}
