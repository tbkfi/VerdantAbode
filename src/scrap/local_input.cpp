#include "local_input.hpp"

#include "FreeRTOS.h"
#include "queue.h"

void gpio_callback(uint gpio, uint32_t events) {
	BaseType_t overtaken = pdFALSE;

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
