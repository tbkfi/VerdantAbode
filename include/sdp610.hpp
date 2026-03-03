// Sensirion SDP610
// Pressure sensor via I2C
//
// [Example usage]
// static SDP610::PARAM sdp610_param;
// sdp610_param.sys = &system_state;
// sdp610_param.que = xQueueCreate(8, sizeof(SDP610::MEASUREMENT));
#pragma once

#include "stdint.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "system.hpp"

namespace SDP610 {
	constexpr unsigned ADDR = 0x40;
	constexpr uint8_t CMD_MEASURE = 0xF1;

	constexpr unsigned POLL_MS = 10 * 1000;      // How often we wish to poll
	constexpr unsigned INTEGRATION_TIME_MS = 8;  // Typical: 4.6ms
	constexpr uint8_t SCALE_FACTOR = 240;        // 60 / 240 / 1200, depending on SKU
	constexpr float CORRECTION_FACTOR = 966.f / 1016.f; // Ambient % Calibration

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
