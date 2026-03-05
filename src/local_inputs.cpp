#include "local_inputs.hpp"
#include "projdefs.h"
#include <cstdint>
#include <hardware/gpio.h>


// Que must be visible within ISR!
static QueueHandle_t que = xQueueCreate(LOCAL_INPUTS::QUE_LEN, sizeof(LOCAL_INPUTS::QUE_ELEMENT));
void isr_local_input(uint gpio, uint32_t events) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if ((events & GPIO_IRQ_EDGE_FALL)) {
		LOCAL_INPUTS::QUE_ELEMENT e;
		e.time_ms = pdTICKS_TO_MS(xTaskGetTickCountFromISR());
		e.pin = gpio;
		
		printf("ISR: %u\n", gpio);
		xQueueSendFromISR(que, &e, &xHigherPriorityTaskWoken);
	}
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


void init_btn(uint8_t pin) {
	gpio_init(pin); gpio_set_dir(pin, GPIO_IN); gpio_pull_up(pin);
	gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL, true, &isr_local_input);
}

void init_rot(uint8_t pin_a, uint8_t pin_b, uint8_t pin_sw) {
	gpio_init(pin_a); gpio_set_dir(pin_a, GPIO_IN); gpio_disable_pulls(pin_a);
	gpio_init(pin_b); gpio_set_dir(pin_b, GPIO_IN); gpio_disable_pulls(pin_b);
	gpio_set_irq_enabled_with_callback(pin_b, GPIO_IRQ_EDGE_FALL, true, &isr_local_input);

	// If rotary has a switch button
	if (pin_sw) {
		gpio_set_dir(pin_sw, GPIO_IN); gpio_init(pin_sw); gpio_pull_up(pin_sw);
		gpio_set_irq_enabled_with_callback(pin_sw, GPIO_IRQ_EDGE_FALL, true, &isr_local_input);
	}
}

QueueHandle_t create_local_inputs(void) {
	// Queue Validation, Registration
	if (que == NULL) {
		while (true) printf("[LOCAL_INPUTS] Queue is NULL!\n");
	}
	vQueueAddToRegistry(que, "LOCAL_INPUTS");

	// Initialise enabled inputs
	if (LOCAL_INPUTS::ENABLE_BTN1) init_btn(LOCAL_INPUTS::BTN1_PIN);
	if (LOCAL_INPUTS::ENABLE_BTN2) init_btn(LOCAL_INPUTS::BTN2_PIN);
	if (LOCAL_INPUTS::ENABLE_BTN3) init_btn(LOCAL_INPUTS::BTN3_PIN);
	if (LOCAL_INPUTS::ENABLE_ROT)
		init_rot(LOCAL_INPUTS::ROTA_PIN, LOCAL_INPUTS::ROTB_PIN, LOCAL_INPUTS::ROTC_PIN);

	return que;
}
