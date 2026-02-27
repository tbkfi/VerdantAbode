#pragma once


namespace Config {
	// General
	constexpr unsigned SENSOR_ARR_LEN = 24;
	constexpr unsigned I2C_INSTANCE_DEFAULT = 1;
	constexpr unsigned I2C_SDA_DEFAULT = 14;
	constexpr unsigned I2C_SCL_DEFAULT = 15;

	// Modbus Addr.
	constexpr unsigned GMP252_ADDR = 240; // CO2
	constexpr unsigned HMP60_ADDR = 241;  // RH
	constexpr unsigned PRODUAL_ADDR = 1;  // Exhaust Fan

	// I2C Addr.
	constexpr unsigned SDP610_ADDR = 0x40; // P
	constexpr unsigned EEPROM_ADDR = 0x50;
}
