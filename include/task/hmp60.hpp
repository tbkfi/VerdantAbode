/* VerdantAbode
 * hmp60.hpp
 *
 * Tuomo Björk
*/
#pragma once

#include "hmp60.hpp"
#include <cstring>
#include <stdint.h>
#include <memory>
#include <cstring>
#include <bit>

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "uart.hpp"
#include "modbus_client.hpp"
#include "modbus_register.hpp"


namespace HMP60 {
	constexpr bool DEBUG = false; // Print debugs?

	namespace REGISTER {
		// docs: https://www.fondriest.com/pdf/vaisala_hmp60-110_manual.pdf
		constexpr uint32_t Rh = 0x0000; // 2 consecutive (lsw+msw)
		constexpr uint32_t T = 0x0002;  // 2 consecutive (lsw+msw)
	};

	constexpr uint16_t ADDRESS = 241;
	constexpr uint16_t POLL_INTERVAL_MS = 500;

	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 3;
	constexpr uint16_t STACK_DEPTH = 1024;
	constexpr uint8_t QUE_LEN = 16;

	struct CTX {
	// Task Context
		std::shared_ptr<ModbusClient> rtu_client;
		std::shared_ptr<PicoUart> uart;
		SemaphoreHandle_t mutex;
		QueueHandle_t que;
        QueueHandle_t que_rh;
	};

	struct QUE_ELEMENT {
	// Individual measurements
		uint32_t time_ms;
		float data_t;  // temperature
		float data_rh; // relative humidity
	};

	void task(void *param);
	QueueHandle_t create_task(SemaphoreHandle_t mutex_uart,
				std::shared_ptr<ModbusClient> rtu_client, std::shared_ptr<PicoUart> uart);
}

