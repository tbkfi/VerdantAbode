// I2C driver
#pragma once
#include "namespace/config.hpp"

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#include <cstring>
#include <stdio.h>

class I2CController {
private:
//	// I2C_DEFAULT is defined in config.hpp 
//	// get the i2c_inst_t from I2C_DEFAULT
//	i2c_inst_t i2c_bus = I2C_INSTANCE(I2C_INSTANCE_DEFAULT);
//	uint i2c_sda = I2C_SDA_DEFAULT;
//	uint i2c_scl = I2C_SCL_DEFAULT;

	uint i2c_bus;
	uint i2c_sda;
	uint i2c_scl;

public:
	// default argument initialization
	I2CController(
		uint i2c_bus_num = Config::I2C_INSTANCE_DEFAULT, 
		uint sda_pin = Config::I2C_SDA_DEFAULT, 
		uint scl_pin = Config::I2C_SCL_DEFAULT);

	// send data
	// receive data
	int read(uint8_t, void*, size_t, uint = 0);
	int write(uint8_t, const void*, size_t, uint = 0);
};

