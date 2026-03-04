#include "util.hpp"
#include "pin.hpp"

#include <cstdint>
#include <hardware/gpio.h>
#include <hardware/structs/io_bank0.h>
#include <pico/time.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "projdefs.h"

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




/*
void init_inputs(void) {
	gpio_init(Pin::ROTA); gpio_set_dir(Pin::ROTA, GPIO_IN); gpio_disable_pulls(Pin::ROTA);
	gpio_init(Pin::ROTB); gpio_set_dir(Pin::ROTB, GPIO_IN); gpio_disable_pulls(Pin::ROTB);
	gpio_init(Pin::ROTC); gpio_set_dir(Pin::ROTC, GPIO_IN); gpio_pull_up(Pin::ROTC);

	gpio_set_irq_enabled_with_callback(Pin::ROTA, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
	gpio_set_irq_enabled_with_callback(Pin::ROTC, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
}
*/
