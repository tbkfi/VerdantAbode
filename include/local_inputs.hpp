#pragma once

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "queue.h"


namespace LOCAL_INPUTS {
	// Enabled local inputs
	constexpr bool ENABLE_BTN1 = true;
	constexpr bool ENABLE_BTN2 = true;
	constexpr bool ENABLE_BTN3 = true;
	constexpr bool ENABLE_ROT = true;

	// Pin assignments
	constexpr uint8_t BTN1_PIN = 7;
	constexpr uint8_t BTN2_PIN = 8;
	constexpr uint8_t BTN3_PIN = 9;
	
	constexpr uint8_t ROTA_PIN = 10;
	constexpr uint8_t ROTB_PIN = 11;
	constexpr uint8_t ROTC_PIN = 12;

	constexpr uint16_t DEBOUNCE_MS = 50;

	// Queue
	constexpr uint8_t QUE_LEN = 16;
	struct QUE_ELEMENT {
		uint8_t pin;
		uint32_t time_ms;
	};
}

void isr_local_input(uint gpio, uint32_t events);

void init_btn(uint8_t pin);
void init_rot(uint8_t pin_a, uint8_t pin_b, uint8_t pin_sw);

QueueHandle_t create_local_inputs(void);
