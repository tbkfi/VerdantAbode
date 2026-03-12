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
	constexpr const char CHARACTERS[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+-=";
	constexpr uint8_t CHARACTERS_LEN = sizeof(WIFI::CHARACTERS) - 1;
}

void action_wifi_setup(SYSTEM::DATA* ctx, LOCAL_INPUTS::QUE_ELEMENT e);
