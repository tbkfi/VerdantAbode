#include "eeprom.hpp"


bool eeprom_read(uint16_t address, uint8_t *buffer, int count) {
// Read 'n' bytes to buffer from EEPROM
	uint8_t mem_address[2];
	int response = 0;

	mem_address[0] = (address >> 8) & 0xFF;
	mem_address[1] = address & 0xFF;

	response = i2c_write_blocking(EEPROM::I2C_UNIT, EEPROM::ADDR, mem_address, 2, true);
	if (response != 2) {
		return false;
	}
	else {
		response = i2c_read_blocking(EEPROM::I2C_UNIT, EEPROM::ADDR, buffer, count, false);
		return (response == count);
	}
}

bool eeprom_write(uint16_t address, uint8_t *data, int count) {
// Write 'n' bytes from buffer to EEPROM
	if (count > TX_BUF_LEN) return false;

	tx_buffer[0] = (address >> 8) & 0xFF;
	tx_buffer[1] = address & 0xFF;
	memcpy(&tx_buffer[2], data, count);

	int response = i2c_write_blocking(EEPROM::I2C_UNIT, EEPROM::ADDR, tx_buffer, 2+count, false);
	sleep_ms(EEPROM::OP_DELAY_MS);

	return (response == 2+count);
}
