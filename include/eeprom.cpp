#include <stdint.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"


Eeprom::Eeprom(I2CController* i2c0, uint8_t dev_addr0, uint8_t op_delay_ms0 = 5) 
: i2c(i2c0), dev_addr(dev_addr0), op_delay_ms(op_delay_ms0) {}

bool Eeprom::read(uint16_t address, uint8_t *buffer, int count) {
// Read 'n' bytes to buffer from EEPROM
	uint8_t mem_address[2];
	int response = 0;

	mem_address[0] = (address >> 8) & 0xFF;
	mem_address[1] = address & 0xFF;

	response = i2c_write_blocking(i2c, dev_addr, mem_address, 2, true);
	if (response != 2) {
		return false;
	}
	else {
		response = i2c_read_blocking(i2c, dev_addr, buffer, count, false);
		return (response == count);
	}
}

bool Eeprom::write(uint16_t address, uint8_t *data, int count) {
// Write 'n' bytes from buffer to EEPROM
	if (count > TX_BUF_LEN) return false;

	tx_buffer[0] = (address >> 8) & 0xFF;
	tx_buffer[1] = address & 0xFF;
	memcpy(&tx_buffer[2], data, count);

	int response = i2c_write_blocking(i2c, dev_addr, tx_buffer, 2+count, false);
	sleep_ms(op_delay_ms);

	return (response == 2+count);
}
