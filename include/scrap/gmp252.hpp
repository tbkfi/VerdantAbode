// Vaisala GMP252
// CO2 sensor via Modbus
//
// [Example usage]
// static GMP252::PARAM gmp252_param;
// gmp252_param.sys = &system_state;
// gmp252_param.que = xQueueCreate(8, sizeof(GMP252::MEASUREMENT));
#pragma once

#include "stdint.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "system.hpp"

namespace GMP252 {
	constexpr unsigned ADDR = 240; // CO2

	struct MEASUREMENT {
	// Individual measurements
		uint32_t time_ms;
		int16_t data;
	};

	struct PARAM {
	// Task parameters
		SYS::STATE *sys;
		QueueHandle_t que;
	};
}

void task_sdp610(void *param);
