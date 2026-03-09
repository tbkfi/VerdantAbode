// I2C driver
#pragma once
#include "namespace/system.hpp"

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#include <cstring>
#include <stdio.h>

class I2CController {
private:
	uint i2c_bus;
	uint i2c_sda;
	uint i2c_scl;

public:
	// default argument initialization
	I2CController(
		uint i2c_bus_num = SYSTEM::I2C::INSTANCE_DEFAULT, 
		uint sda_pin = SYSTEM::I2C::SDA_DEFAULT, 
		uint scl_pin = SYSTEM::I2C::SCL_DEFAULT);

	// send data
	// receive data
	int read(uint8_t, void*, size_t, uint = 0);
	int write(uint8_t, const void*, size_t, uint = 0);
};

