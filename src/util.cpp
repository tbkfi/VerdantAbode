/* VerdantAbode
 * util.cpp
 *
 * Tuomo Björk
*/
#include "util.hpp"


void init_i2c0(void) {
	// Eeprom
	i2c_init(Pin::I2C0_UNIT, Pin::I2C0_BAUD);
	gpio_set_function(Pin::I2C0_SCL, GPIO_FUNC_I2C);
	gpio_set_function(Pin::I2C0_SDA, GPIO_FUNC_I2C);
	printf("[INIT] I2C0 Initialised\n");
}

void init_i2c1(void) {
	// SDP610
	i2c_init(Pin::I2C1_UNIT, Pin::I2C1_BAUD);
	gpio_set_function(Pin::I2C1_SCL, GPIO_FUNC_I2C);
	gpio_set_function(Pin::I2C1_SDA, GPIO_FUNC_I2C);
	printf("[INIT] I2C1 Initialised\n");
}
