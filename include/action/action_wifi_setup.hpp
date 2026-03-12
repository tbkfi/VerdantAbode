/* VerdantAbode
 * action_wifi_setup.hpp
 *
 * Tuomo Björk
*/
#pragma once

#include <string.h>
#include "FreeRTOS.h"
#include "system.hpp"
#include "local_inputs.hpp"


namespace WIFI {
	// Selectable characters

	//constexpr const char CHARACTERS[] = 
    //    "0123456789!@#$%^&*()_+-= ";

    /*
    char CHAR_ROW_ALPHA_LOWER[] =
        "abcdefghijklmnopqrstuvwxyz";

    char CHAR_ROW_ALPHA_UPPER[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        
    char CHAR_ROW_PUNCT[] =
        "!\"#$%&'()*+,\\-./:;<=>?@[]^_`{|}~";

    char CHAR_ROW_CONTROL[] =
        "<%NP";

    char* CHAR_ROWS[] = {
        CHAR_ROW_ALPHA_LOWER,
        CHAR_ROW_ALPHA_UPPER,
        CHAR_ROW_PUNCT,
        CHAR_ROW_CONTROL
    };
    */

    constexpr const int CHAR_ROW_COUNT = 4;

	//constexpr uint8_t CHARACTERS_LEN = sizeof(WIFI::CHARACTERS) - 1;
}

void action_wifi_setup(SYSTEM::DATA* ctx, LOCAL_INPUTS::QUE_ELEMENT e);
