// I2C driver
#pragma once
#include "namespace/config.hpp"

class I2CBus {
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
	I2CBus() :
		i2c_bus(I2C_INSTANCE(Config::I2_INSTANCE_DEFAULT)),
		i2c_sda(Config::I2C_SDA_DEFAULT),
		i2c_scl(Config::I2C_SCL_DEFAULT)
	{};

	// send data
	// receive data
};

// constructor for i2c bus. Inits i2cX and SDA/SCL pins
I2CBus::I2CBus(uint i2c_bus_num, uint sda_pin, uint scl_pin) :
	i2c_bus(I2C_INSTANCE(i2c_bus_num)), i2c_sda(i2c_sda), i2c_scl(i2c_scl)
{
	i2c_init(i2c_bus, 100 * 1000)
	gpio_set_function(i2c_sda, GPIO_FUNC_I2C);
	gpio_set_function(i2c_scl, GPIO_FUNC_I2C);
	gpio_pull_up(i2c_sda);
	gpio_pull_up(i2c_scl);
}
