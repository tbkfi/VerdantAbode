#pragma once

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

namespace SYSTEM {
	struct DATA {
	// System State
		uint32_t co2_target;		  // CO2 Level
		SemaphoreHandle_t mutex_i2c;  // I2C Lock
		QueueHandle_t input_queue;    // Local inputs
		QueueHandle_t sdp610_queue;	  // SDP610::PARAM->que
		QueueHandle_t gmp252_queue;	  // GMP252::PARAM->que
	};
}
