#pragma once


namespace Config {
	// Modbus Addr.
	const unsigned GMP252_ADDR = 240; // CO2
	const unsigned HMP60_ADDR = 241;  // RH
	const unsigned PRODUAL_ADDR = 1;  // Exhaust Fan

	// I2C Addr.
	const unsigned SDP610_ADDR = 0x40; // P
	const unsigned EEPROM_ADDR = 0x50;
}
