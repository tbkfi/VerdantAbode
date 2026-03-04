#pragma once

#include <stdint.h>
#include "hardware/i2c.h"

#include "driver/i2c.hpp"


class Eeprom {
private:
	static constexpr size_t TX_BUF_LEN = 128;
	I2CController* i2c;
	uint8_t dev_addr;
	uint8_t op_delay_ms;
	uint8_t tx_buffer[TX_BUF_LEN + 2];

public:
	Eeprom(I2CController* i2c0, uint8_t dev_addr0, uint8_t op_delay_ms0 = 5);

	bool read(uint16_t address, uint8_t *buffer, int count);
	bool write(uint16_t address, uint8_t *data, int count);
};
