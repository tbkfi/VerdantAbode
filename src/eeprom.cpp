/* VerdantAbode
 * eeprom.cpp
 *
 * Tuomo Björk
*/
#include "eeprom.hpp"
#include "portmacro.h"
#include "projdefs.h"


bool EEPROM::read(uint16_t address, uint8_t *buffer, int count) {
	uint8_t mem_address[2];
	mem_address[0] = (address >> 8) & 0xFF;
	mem_address[1] = address & 0xFF;

	int response = i2c_write_timeout_us(EEPROM::I2C, EEPROM::ADDRESS, mem_address, 2, true, EEPROM::I2C_TIMEOUT_US);
	if (response == 2) {
		response = i2c_read_timeout_us(EEPROM::I2C, EEPROM::ADDRESS, buffer, count, false, EEPROM::I2C_TIMEOUT_US);
		if (response == count) return true;
	}

	printf("[EEPROM] (read): fail (%d)!\n", response);
	return false;
}

bool EEPROM::write(uint16_t address, uint8_t *buffer, int count) {
	uint8_t data[EEPROM::PAGE_SIZE + 2]; 
	if (count > EEPROM::PAGE_SIZE) return false;

	data[0] = (address >> 8) & 0xFF;
	data[1] = address & 0xFF;
	memcpy(&data[2], buffer, count);

	int response = i2c_write_timeout_us(EEPROM::I2C, EEPROM::ADDRESS, data, 2 + count, false, EEPROM::I2C_TIMEOUT_US);
	vTaskDelay(pdMS_TO_TICKS(EEPROM::OP_DELAY_MS));

	if (response == 2 + count) return true;
	else {
		printf("[EEPROM] (write): fail (%d)!\n", response);
		return false;
	}
}

bool EEPROM::save(SYSTEM::DATA* ctx, SemaphoreHandle_t mutex_i2c) {
	EEPROM::TABLE data;
	
	// Table data here
	data.magic = ctx->magic;
	data.val_co2 = ctx->val_co2;
	
	// CRC
	size_t data_len = sizeof(EEPROM::TABLE) - sizeof(uint16_t);
	data.crc = crc16((uint8_t*) &data, data_len);

	// Write
	bool status = false;
	if (xSemaphoreTake(mutex_i2c, portMAX_DELAY) == pdTRUE) {
		status = EEPROM::write(EEPROM::DATA_ADDR, (uint8_t*)&data, sizeof(EEPROM::TABLE));
		xSemaphoreGive(mutex_i2c);
	}

	if (!status) printf("[EEPROM] (save): failed to save data!\n");
	else printf("[EEPROM] (save): data saved succesfully!\n");

	return status;
}

bool EEPROM::load(SYSTEM::DATA* ctx, SemaphoreHandle_t mutex_i2c) {
	EEPROM::TABLE data;

	// Get the data
	bool status = false;
	if (xSemaphoreTake(mutex_i2c, portMAX_DELAY) == pdTRUE) {
		status = EEPROM::read(EEPROM::DATA_ADDR, (uint8_t*) &data, sizeof(EEPROM::TABLE));
		xSemaphoreGive(mutex_i2c);
	}

	if (!status) {
		// Failure
		printf("[EEPROM] (load): Reading failed!\n");
		return false;
	}
	
	// Magic
	if (data.magic != SYSTEM::MAGIC_BYTE) {
		printf("[EEPROM] (load): Magic failed!\n");
		return false;
	}

	// CRC
	size_t data_len = sizeof(EEPROM::TABLE) - sizeof(uint16_t);
	uint16_t crc = crc16((uint8_t*) &data, data_len);
	if (crc != data.crc) {
		printf("[EEPROM] (load): CRC failed!\n");
		return false;
	}

	// 4. Recovery successful, apply to context
	ctx->magic = data.magic;
	ctx->val_co2 = data.val_co2;

	printf("[EEPROM] (load): Restored state from EEPROM\n");
	return true;
}

uint16_t EEPROM::crc16(const uint8_t *pData, size_t len) {
// Yoinked from Lab5 instructions.
	uint8_t x;
	uint16_t crc = 0xFFFF;
	while (len--) {
		x = crc >> 8 ^ *pData++;
		x ^= x >> 4;
		crc = (crc << 8) ^ ((uint16_t) (x << 12)) ^ ((uint16_t) (x << 5)) ^ ((uint16_t) x);
	}
	return crc;
}
