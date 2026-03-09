#pragma once

#include <stdio.h>
#include <stdint.h>

namespace EEPROM {
	constexpr uint8_t ADDR = 0x50;
	constexpr uint8_t OP_DELAY_MS = 5;
}

bool eeprom_read(uint16_t address, uint8_t *buffer, int count);
bool eeprom_write(uint16_t address, uint8_t *data, int count);
