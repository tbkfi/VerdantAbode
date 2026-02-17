// I2C driver
#pragma once
#include "namespace/config.hpp"
#include "hardware/i2c.h"

class I2CController {
private:
//	// I2C_DEFAULT is defined in config.hpp 
//	// get the i2c_inst_t from I2C_DEFAULT
//	i2c_inst_t i2c_bus = I2C_INSTANCE(I2C_INSTANCE_DEFAULT);
//	uint i2c_sda = I2C_SDA_DEFAULT;
//	uint i2c_scl = I2C_SCL_DEFAULT;

	i2c_inst_t i2c_bus;
	uint i2c_sda;
	uint i2c_scl;

public:
	// I2C_INSTANCE is a pico-sdk macro to get i2c_inst_t inst # (can be 1/0)
	// default argument initialization
	I2CController() :
		i2c_bus(I2C_INSTANCE(Config::I2_INSTANCE_DEFAULT)),
		i2c_sda(Config::I2C_SDA_DEFAULT),
		i2c_scl(Config::I2C_SCL_DEFAULT)
	{};

	// send data
	// receive data
};

