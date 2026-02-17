
#include "namespace/config.hpp"
#include "driver/i2c.hpp"
#include "hardware/i2c.h"

// I2C driver
// constructor for i2c bus. Inits i2cX and SDA/SCL pins
I2CController::I2CController(uint i2c_bus_num, uint sda_pin, uint scl_pin) :
	i2c_bus(I2C_INSTANCE(i2c_bus_num)), i2c_sda(i2c_sda), i2c_scl(i2c_scl)
{
	i2c_init(i2c_bus, 100 * 1000)
	gpio_set_function(i2c_sda, GPIO_FUNC_I2C);
	gpio_set_function(i2c_scl, GPIO_FUNC_I2C);
	gpio_pull_up(i2c_sda);
	gpio_pull_up(i2c_scl);
}

// pico-sdk definitions :
// 	int i2c_write_blocking (i2c_inst_t * i2c, uint8_t addr, const uint8_t * src, size_t len, bool nostop)
// 	int i2c_read_blocking (i2c_inst_t * i2c, uint8_t addr, uint8_t * dst, size_t len, bool nostop)
int I2CController::read(uint8_t slave_addr, void* dst, size_t len, off_t offset)
// read(slave_addr, destination, how many bytes, offset)
{
	uint8_t     dst_str;
	uint8_t     address[2];

	address[0] = ((offset & 0xFF00) >> 8);     // extract hi bits
	address[1] = (uint8_t)(offset & 0x00FF);   // extract lo bits

	i2c_write_blocking( i2c0, addr_slave, address, 2, 1 );
	sleep_ms(5);
	i2c_read_blocking ( i2c0, addr_slave, &dst_str, 1, 0 );

	return dst_str; 
}
