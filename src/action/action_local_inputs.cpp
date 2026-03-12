/* VerdantAbode
 * action_local_input.cpp
 *
 * Tuomo Björk
*/
#include "action_local_inputs.hpp"
#include "action_wifi_setup.hpp"
#include "system.hpp"

#define __DEBUG_THIS_FILE__ true
#ifndef __DEBUG_THIS_FILE__
#define __DEGUG_THIS_FILE__ false
#endif

void action_local_input_regular
(SYSTEM::DATA* ctx, LOCAL_INPUTS::QUE_ELEMENT* e)
{
// Action(s) taken when a LOCAL INPUT is received.
    ctx->current_view = SYSTEM::VIEW::MAIN;

	printf("[%lu] PARSER(local_input): %u\n", e->time_ms, e->pin);
    //DBG_PRINT(__DEBUG_THIS_FILE__, "LOCAL_INPUT", "Caught input: %u", e->pin)

	switch (e->pin) {
		default:
		// Undefined
			break;

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
            fan_speed_target(ctx, (ctx->val_fan += 100), 5);
			break;

		case LOCAL_INPUTS::BTN3_PIN:
            fan_speed_target(ctx, (ctx->val_fan -= 100), 5);
			break;

		case LOCAL_INPUTS::BTN2_PIN:
        { // Toggle MAIN | SETUP state
			EventBits_t events = xEventGroupGetBits(ctx->events);
			if (events & SYSTEM::FLAG_WIFI_SETUP) {
				xEventGroupClearBits(ctx->events, SYSTEM::FLAG_WIFI_SETUP);
			} else {
				xEventGroupSetBits(ctx->events, SYSTEM::FLAG_WIFI_SETUP);
			}
			break;
		}
	}

}

// this just handles input when the current screen is WIFI_SETUP
void action_local_input_wifi_setup
(SYSTEM::DATA* ctx, LOCAL_INPUTS::QUE_ELEMENT* e)
{
    ctx->current_view = SYSTEM::VIEW::WIFI_SETUP;

	//printf("[%lu] WIFI_SETUP(local_input): %u\n", e->time_ms, e->pin);
    switch (e->pin)
    {
    case LOCAL_INPUTS::ROTA_PIN:
        // cursor ++
        DBG_PRINT(__DEBUG_THIS_FILE__, "WIFI_SETUP:EVENT", "%u", e->pin)

        // if column is less than the length of the current row
        // rows are defined in 'action/action_wifi_setup.hpp'
        // ctx->wifi_setup_{column,row} are defined in system.hpp
        if (ctx->wifi_setup_column < strlen(SYSTEM::CHARSET::ASCII[ctx->wifi_setup_row]))
            ctx->wifi_setup_column ++;
        else
            ctx->wifi_setup_column = 0;
    
        break;

    case LOCAL_INPUTS::ROTB_PIN:
        DBG_PRINT(__DEBUG_THIS_FILE__, "WIFI_SETUP:EVENT", "%u", e->pin)
        // cursor --
        // strlen())
        if (ctx->wifi_setup_column > 0)
            ctx->wifi_setup_column --;
        else
            ctx->wifi_setup_column = strlen(SYSTEM::CHARSET::ASCII[ctx->wifi_setup_row]);

        break;

    case LOCAL_INPUTS::ROTC_PIN:
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
                break;
            }

        } else {

            if (ctx->wifi_setup_field == 0)
                ctx->SSID.push_back(c);
            else
                ctx->PASSWORD.push_back(c);
        }

        DBG_PRINT(__DEBUG_THIS_FILE__, "WIFI_SETUP:INPUT", "%s", ctx->SSID.c_str());
        break;
    }
    case LOCAL_INPUTS::BTN3_PIN:
        DBG_PRINT(__DEBUG_THIS_FILE__, "WIFI_SETUP:EVENT", "%u", e->pin)
        // row ++

        //                      4 rows indexed 0 - 3
        if (ctx->wifi_setup_row < SYSTEM::CHARSET::ROW_COUNT)
            ctx->wifi_setup_row ++;
        else
            ctx->wifi_setup_row = 0;

        // if curorsor is beyond current row limit
        if (ctx->wifi_setup_column > strlen(SYSTEM::CHARSET::ASCII[ctx->wifi_setup_row]))
            ctx->wifi_setup_column = strlen(SYSTEM::CHARSET::ASCII[ctx->wifi_setup_row]);

        break;

    case LOCAL_INPUTS::BTN2_PIN:
        // quit wifi setup
        DBG_PRINT(__DEBUG_THIS_FILE__, "WIFI_SETUP:QUIT", "%u", e->pin)
        xEventGroupClearBits(ctx->events, SYSTEM::FLAG_WIFI_SETUP);
        break;

    case LOCAL_INPUTS::BTN1_PIN:
        DBG_PRINT(__DEBUG_THIS_FILE__, "WIFI_SETUP:EVENT", "%u", e->pin)
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
