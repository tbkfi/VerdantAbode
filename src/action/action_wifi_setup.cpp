/* VerdantAbode
 * action_wifi_setup.cpp
 *
 * Tuomo Björk
*/
#include "action/action_wifi_setup.hpp"
#include "system.hpp"


void wifi_setup_task(void* param) {
	SYSTEM::DATA* sys = (SYSTEM::DATA*) param;
	LOCAL_INPUTS::QUE_ELEMENT e;

	int index = 0;
	uint32_t flags = 0;
	while (true) {
		flags = xEventGroupGetBits(sys->events);

		// Finish Setup
		if (e.pin == LOCAL_INPUTS::BTN3_PIN) {
			xEventGroupClearBits(sys->events, SYSTEM::FLAG_WIFI_SETUP);
			// Also: EEPROM SAVE ?
		}

		// Toggle Selected Field (SSID | PASS)
		if (e.pin == LOCAL_INPUTS::BTN1_PIN) {
			if (flags & SYSTEM::FLAG_WIFI_PFIELD) {
				// Movde to SSID field
				xEventGroupClearBits(sys->events, SYSTEM::FLAG_WIFI_PFIELD);
			} else {
				// Move to PASS field
				xEventGroupSetBits(sys->events, SYSTEM::FLAG_WIFI_PFIELD);
			}
		}

		// Select Character
		if (e.pin == LOCAL_INPUTS::ROTC_PIN) {
			if (flags & xEventGroupGetBits(sys->events)) {
				// Manipulate PASS input
				if (sys->wifi_pass_pos < SYSTEM::SSID_BUFF_LEN) {
					sys->wifi_pass[sys->wifi_pass_pos] = WIFI::CHARACTERS[index];
					sys->wifi_pass[sys->wifi_pass_pos] = '\0';
				}
			} else {
				// Manipulate SSID input
				if (sys->wifi_ssid_pos < SYSTEM::SSID_BUFF_LEN) {
					sys->wifi_ssid[sys->wifi_ssid_pos++] = WIFI::CHARACTER[index];
					sys->wifi_ssid[sys->wifi_ssid_pos] = '\0';
				}
			}
		}

		// Move Clockwise
		if (e.pin == LOCAL_INPUTS::ROTA_PIN) {
			index = (index + 1) % WIFI::CHARACTERS_LEN;
			sys->setup_c = WIFI::CHARACTERS[index];
		}

		// Move Counter Clockwise
		if (e.pin == LOCAL_INPUTS::ROTB_PIN) {
			index = (index - 1 + WIFI::CHARACTERS_LEN) % WIFI::CHARACTERS_LEN;
			sys->setup_c = WIFI::CHARACTERS[index];
		}
	}
}
