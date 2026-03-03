#include "util.hpp"
#include "pin.hpp"

#include <cstdint>
#include <hardware/gpio.h>
#include <hardware/structs/io_bank0.h>
#include <pico/time.h>
#include <stdlib.h>

#include "pico/stdlib.h"

/*
void gpio_callback(uint gpio, uint32_t events) {
    BaseType_t overtaken = pdFALSE; // Higher Prio?

    event_input event;
    event.timestamp = xTaskGetTickCountFromISR();
    if ((events & GPIO_IRQ_EDGE_FALL)) {
        if (gpio == Pin::ROTC) {
            event.action = InputAction::PRESS;
            xQueueSendFromISR(inputs, &event, &overtaken);
        }
        else if (gpio == Pin::ROTA) {
            if (gpio_get(Pin::ROTB)) event.action = InputAction::RIGHT;
            else event.action = InputAction::LEFT;
            xQueueSendFromISR(inputs, &event, &overtaken);
        }
    }
    portYIELD_FROM_ISR(overtaken);
}
*/

uint32_t uptime_ms(void) {
// Time since boot (Pico-SDK)
	return to_ms_since_boot(get_absolute_time());
}

void init_i2c(void) {
	// Eeprom
	i2c_init(Pin::I2C0_UNIT, Pin::I2C0_BAUD);
	gpio_set_function(Pin::I2C0_SCL, GPIO_FUNC_I2C);
	gpio_set_function(Pin::I2C0_SDA, GPIO_FUNC_I2C);
	printf("[%lu] I2C0 Initialised\n", uptime_ms());

	// SDP610
	i2c_init(Pin::I2C1_UNIT, Pin::I2C1_BAUD);
	gpio_set_function(Pin::I2C1_SCL, GPIO_FUNC_I2C);
	gpio_set_function(Pin::I2C1_SDA, GPIO_FUNC_I2C);
	printf("[%lu] I2C1 Initialised\n", uptime_ms());
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
