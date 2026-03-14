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
// Action(s) taken when a LOCAL INPUT is received during regular view.
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
		// Save Settings
			EEPROM::save(ctx);
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

void action_local_input_wifi_setup (SYSTEM::DATA* ctx, LOCAL_INPUTS::QUE_ELEMENT* e) {
// Action(s) taken when a LOCAL INPUT is received during wifi_setup view.
// * Pavel Shishkin
	ctx->current_view = SYSTEM::VIEW::WIFI_SETUP;

	//printf("[%lu] WIFI_SETUP(local_input): %u\n", e->time_ms, e->pin);
	switch (e->pin)
	{
	case LOCAL_INPUTS::ROTA_PIN:
		// cursor ++
		//DBG_PRINT(__DEBUG_THIS_FILE__, "WIFI_SETUP:EVENT", "%u", e->pin)

		// if column is less than the length of the current row
		// rows are defined in 'action/action_wifi_setup.hpp'
		// ctx->wifi_setup_{column,row} are defined in system.hpp
		if (ctx->wifi_setup_column < strlen(SYSTEM::CHARSET::ASCII[ctx->wifi_setup_row]))
			ctx->wifi_setup_column ++;
		else
			ctx->wifi_setup_column = 0;
	
		break;

	case LOCAL_INPUTS::ROTB_PIN:
		//DBG_PRINT(__DEBUG_THIS_FILE__, "WIFI_SETUP:EVENT", "%u", e->pin)
		// cursor --
		// strlen())
		if (ctx->wifi_setup_column > 0)
			ctx->wifi_setup_column --;
		else
			ctx->wifi_setup_column = strlen(SYSTEM::CHARSET::ASCII[ctx->wifi_setup_row]);

		break;

	case LOCAL_INPUTS::ROTC_PIN:
		// unused
		break;

	case LOCAL_INPUTS::BTN3_PIN:

		// quit wifi setup
		//DBG_PRINT(__DEBUG_THIS_FILE__, "WIFI_SETUP:QUIT", "%u", e->pin)
		xEventGroupClearBits(ctx->events, SYSTEM::FLAG_WIFI_SETUP);

		break;

/*
		DBG_PRINT(__DEBUG_THIS_FILE__, "WIFI_SETUP:EVENT", "%u", e->pin)
		// row ++

		//					  4 rows indexed 0 - 3
		if (ctx->wifi_setup_row < SYSTEM::CHARSET::ROW_COUNT)
			ctx->wifi_setup_row ++;
		else
			ctx->wifi_setup_row = 0;

		// if curorsor is beyond current row limit
		if (ctx->wifi_setup_column > strlen(SYSTEM::CHARSET::ASCII[ctx->wifi_setup_row]))
			ctx->wifi_setup_column = strlen(SYSTEM::CHARSET::ASCII[ctx->wifi_setup_row]);

*/
	case LOCAL_INPUTS::BTN2_PIN:
	{ // scope

		char c = (SYSTEM::CHARSET::ASCII[ctx->wifi_setup_row][ctx->wifi_setup_column]);

		// control chars
		if (ctx->wifi_setup_row == 6)
		{
			// choose the correct field SSID/PASSWORD
			std::string& ptr = (ctx->wifi_setup_field == 0) ? ctx->SSID : ctx->PASSWORD;
			switch (c)
			{
			case '<':
				ptr.pop_back();
				break;
			case '%':
				ptr.clear();
				break;
			case 'N':
				// toggle the fields
				ctx->wifi_setup_field = (ctx->wifi_setup_field == 0) ? 1 : 0;
				break;
			case '+':
				// TODO: WIFI CONNECTION ROUTINE
				xEventGroupSetBits(ctx->events, SYSTEM::FLAG_WIFI_SETUP_READY);
				break;
			}

		} else {

			if (ctx->wifi_setup_field == 0)
				ctx->SSID.push_back(c);
			else
				ctx->PASSWORD.push_back(c);
		}

		//DBG_PRINT(__DEBUG_THIS_FILE__, "WIFI_SETUP:INPUT", "%s", ctx->SSID.c_str());
		break;
	}
	case LOCAL_INPUTS::BTN1_PIN:
		//DBG_PRINT(__DEBUG_THIS_FILE__, "WIFI_SETUP:EVENT", "%u", e->pin)
		// row --
		if (ctx->wifi_setup_row > 0)
			ctx->wifi_setup_row --;
		else
			ctx->wifi_setup_row = SYSTEM::CHARSET::ROW_COUNT - 1;

		// if curorsor is beyond current row limit
		if (ctx->wifi_setup_column > strlen(SYSTEM::CHARSET::ASCII[ctx->wifi_setup_row]))
			ctx->wifi_setup_column = strlen(SYSTEM::CHARSET::ASCII[ctx->wifi_setup_row]);

		break;
		
	default:
	// Undefined
		break;
	
	}
}
