#pragma once

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

namespace SYSTEM {
	struct DATA {
		uint32_t co2_setpoint;
		SemaphoreHandle_t semaphore_i2c;  // Binary Semaphore
		QueueHandle_t sdp610_queue;       // SDP610::PARAM->que
		QueueHandle_t gmp252_queue;       // GMP252::PARAM->que
	};
}
