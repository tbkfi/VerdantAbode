#pragma once

#include "stdint.h"


namespace Config {
	// General
	constexpr unsigned char DEBOUNCE_MS = 250;
	constexpr unsigned BUFFER_LEN_GENERIC = 16;
	constexpr unsigned SENSOR_ARR_LEN = 24;

	// Timings
	constexpr unsigned WAIT_SEM_MS = 10;
	
	// Modbus Addr.
	constexpr unsigned HMP60_ADDR = 241;  // RH
	constexpr unsigned PRODUAL_ADDR = 1;  // Exhaust Fan

	// I2C Addr.
	constexpr unsigned EEPROM_ADDR = 0x50;
}

