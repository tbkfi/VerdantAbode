
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#include "namespace/config.hpp"
#include "i2c.hpp"

#include <cstring>
#include <stdio.h>


// pico-sdk definitions :
// 	int i2c_write_blocking (i2c_inst_t * i2c, uint8_t addr, const uint8_t * src, size_t len, bool nostop)
// 	int i2c_read_blocking (i2c_inst_t * i2c, uint8_t addr, uint8_t * dst, size_t len, bool nostop)


// I2C driver
// constructor for i2c bus. Inits i2cX and SDA/SCL pins
I2CController::I2CController(uint i2c_bus_num, uint sda_pin, uint scl_pin) :
	i2c_bus(i2c_bus_num), i2c_sda(sda_pin), i2c_scl(scl_pin)
{
	// I2C_INSTANCE is a pico-sdk macro to get i2c_inst_t inst # (can be 1/0)
	i2c_init(I2C_INSTANCE(i2c_bus), 100 * 1000);
	gpio_set_function(i2c_sda, GPIO_FUNC_I2C);
	gpio_set_function(i2c_scl, GPIO_FUNC_I2C);
	gpio_pull_up(i2c_sda);
	gpio_pull_up(i2c_scl);
}

int I2CController::read(uint8_t slave_addr, void* dst, size_t nbyte, uint offset)
// example: read(0x40, uint8_t*, 1, 0x20)
{
	uint8_t     address[2];

	// first 2 bytes of the packet is the address to write to
	uint16_t off_uint16t = offset;
	memcpy(address, &off_uint16t, 2); // set the address in the packet
	
	int b = 0;
	b = i2c_write_blocking(I2C_INSTANCE(i2c_bus), slave_addr, address, sizeof(address), true);

	if (b <= 0)
		printf("No bytes written!\n");
	sleep_ms(5);

	b = i2c_read_blocking(I2C_INSTANCE(i2c_bus), slave_addr, (uint8_t*)dst, nbyte, false);
	if (b <= 0)
		printf("No bytes read!\n");

	return b; 
}

int I2CController::write(uint8_t slave_addr, const void* buffer, size_t nbyte, uint offset)
// example: write(0x40, uint8_t*, 1, 0x20)
{
	// account for address
	uint8_t packet[nbyte + 2];

 	// offset to 2 bytes, account for address
	memcpy(packet + 2, (void*)buffer, nbyte);
	
	// first 2 bytes of the packet is the address to write to
	uint16_t off_uint16t = offset;
	memcpy(&off_uint16t, packet, 2); // set the address in the packet

//printf("PGWRITE >> %u %u %u %u \n", packet[2], packet[3], packet[4], packet[5]);
//printf("PGWRITE >> 0x%04X SIZE: [%d]\n", packet, payload_size);

	int stat = i2c_write_blocking(I2C_INSTANCE(i2c_bus), slave_addr, packet, nbyte + 2, false); 
	if (stat <= 0)
		printf("No bytes written!\n");

	sleep_ms(10);
	return stat;
}

