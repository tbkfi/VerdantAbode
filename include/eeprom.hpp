/* VerdantAbode
 * eeprom.hpp
 *
 * Tuomo Björk
*/
#pragma once

#include <stdio.h>
#include <stdint.h>
#include <cstring>

#include "hardware/i2c.h"

#include "FreeRTOS.h"
#include "portmacro.h"
#include "projdefs.h"
#include "task.h"
#include "semphr.h"

#include "system.hpp"


namespace EEPROM {
	struct __attribute__((packed)) TABLE {
	// see: https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Packed-Structures.html
	// SYSTEM::DATA ctx, saved fields go here!
		uint8_t magic;
		int16_t co2_target;
		uint16_t crc;
	};

	inline i2c_inst_t* const I2C = i2c0;
	constexpr uint32_t I2C_TIMEOUT_US = 10 * 1000;
	constexpr uint32_t BAUDRATE_HZ = (100 * 1000);
	constexpr uint8_t SDA_PIN = 16;
	constexpr uint8_t SCL_PIN = 17;

	constexpr uint32_t DATA_ADDR = 0;          // Saved data addr.

	constexpr uint8_t ADDRESS = 0x50;          // EEPROM dev address
	constexpr uint8_t OP_DELAY_MS = 5;         // Write delay
	constexpr uint16_t PAGES = 512;            // EEPROM page count
	constexpr uint8_t PAGE_SIZE = 64;          // EEPROM page size
	constexpr uint8_t PAGE_MEM_SIZE = 8;       // EEPROM page member size
	constexpr uint32_t ADDRESS_TOTAL = 0x8000; // Total memory addresses


	bool read(uint16_t address, uint8_t *buffer, int count);
	bool write(uint16_t address, uint8_t *buffer, int count);

	bool save(SYSTEM::DATA* ctx);
	bool load(SYSTEM::DATA* ctx);

	uint16_t crc16(const uint8_t *pData, size_t len);
}
