#pragma once
#include "i2c.hpp"
#include <memory>

class EEPROM {
public:
	EEPROM(std::shared_ptr<I2CController> i2c_i, uint address) :
		i2c_i(i2c_i), address(address)
	{}

	int read(void* dst, size_t nbyte, uint offset);
	int write(const void* buffer, size_t nbyte, uint offset);
private:
	std::shared_ptr<I2CController> i2c_i;
	uint address;
};
