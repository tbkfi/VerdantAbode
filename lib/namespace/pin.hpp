#pragma once

#include "hardware/i2c.h"

namespace Pin {
	// Local Buttons
	const unsigned char BTN1 = 7;
	const unsigned char BTN2 = 8;
	const unsigned char BTN3 = 9;

	// Local Vent
	const unsigned char EXHAUST_VALVE = 32;

	// Rotary Encoder
	const unsigned char ROT1 = 10;
	const unsigned char ROT2 = 11;
	const unsigned char ROT_SW = 12;

	// Local LED indicators
	const unsigned char LED1 = 20;
	const unsigned char LED2 = 21;
	const unsigned char LED3 = 22;
	
	// UART (Main)
	const unsigned char UART_TX_MASTER = 4;
	const unsigned char UART_RX_MASTER = 5;

	// UART (Simulator)
	const unsigned char UART_TX_SLAVE = 5;
	const unsigned char UART_RX_SLAVE = 4;

	// I2C (Sensirion)
	const i2c_inst_t I2C1_UNIT = i2c1;
	const unsigned char I2C0_SDA = 14;
	const unsigned char I2C0_SCL = 15;

	// I2C (EEPROM)
	const i2c_inst_t I2C0_UNIT = i2c0;
	const unsigned char I2C1_SDA = 16;
	const unsigned char I2C1_SCL = 17;
}

