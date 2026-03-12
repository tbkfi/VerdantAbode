/* VerdantAbode
 * sdp610.hpp
 *
 * Tuomo Björk
*/
#pragma once

#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"


namespace SDP610 {
	constexpr bool DEBUG = false; // Print debugs?

	inline i2c_inst_t* const I2C_UNIT = i2c1;
	constexpr unsigned ADDR = 0x40;
	constexpr uint8_t CMD_MEASURE = 0xF1;
	constexpr uint16_t POLL_INTERVAL_MS = 1500;

	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 3;
	constexpr uint16_t STACK_DEPTH = 2048;
	constexpr uint8_t QUE_LEN = 8;
	constexpr uint32_t I2C_TIMEOUT_US = 10 * 1000;
	constexpr uint8_t INTEGRATION_TIME_MS = 15;          // Typical: 4.6 - 12.4ms
	constexpr uint8_t SCALE_FACTOR = 240;                // 60 | 240 | 1200 (depend on SKU)
	constexpr float CORRECTION_FACTOR = 966.f / 1016.f;  // Ambient % Calibration

	struct CTX {
	// Task Context
		QueueHandle_t que;
		SemaphoreHandle_t mutex;
	};

	struct QUE_ELEMENT {
	// Individual measurements
		uint32_t time_ms;
		float data;
	};
}

void task_sdp610(void *param);
QueueHandle_t task_create_sdp610(SemaphoreHandle_t mutex_i2c);
