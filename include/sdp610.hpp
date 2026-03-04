// Sensirion SDP610 (I2C)
#pragma once

#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

#include "FreeRTOS.h"
#include "projdefs.h"
#include "portmacro.h"
#include "queue.h"
#include "semphr.h"

#include "system.hpp"
#include "pin.hpp"
#include "util.hpp"


namespace SDP610 {
	constexpr bool DEBUG = true; // Print debugs?
	constexpr unsigned ADDR = 0x40;
	constexpr uint8_t CMD_MEASURE = 0xF1;
	constexpr uint16_t POLL_INTERVAL_MS = 1 * 1000;

	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 2;
	constexpr uint16_t STACK_DEPTH = 1024;
	constexpr uint8_t QUE_LEN = 8;
	constexpr uint8_t INTEGRATION_TIME_MS = 8;          // Typical: 4.6ms
	constexpr uint8_t SCALE_FACTOR = 240;               // 60 | 240 | 1200 (depend on SKU)
	constexpr float CORRECTION_FACTOR = 966.f / 1016.f; // Ambient % Calibration

	struct CTX {
	// Task Context
		QueueHandle_t que;
		SemaphoreHandle_t sem;
	};

	struct MEASUREMENT {
	// Individual measurements
		uint32_t time_ms;
		int16_t data;
	};
}

void task_sdp610(void *param);
QueueHandle_t task_create_sdp610(SemaphoreHandle_t mutex_i2c);
