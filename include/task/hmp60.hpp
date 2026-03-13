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
#include <bit>

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include <modbus_client.hpp>
#include <modbus_register.hpp>


namespace HMP60 {
	constexpr bool DEBUG = false; // Print debugs?

	namespace REGISTER {
		// docs: https://www.fondriest.com/pdf/vaisala_hmp60-110_manual.pdf
		constexpr uint32_t T = 0x0002;
        constexpr uint32_t RH = 0x0000;
	};

	constexpr uint16_t ADDRESS = 241;
	constexpr uint16_t POLL_INTERVAL_MS = 1000;
	//constexpr uint16_t INTEGRATION_TIME_MS = 12 * 1000;

	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 3;
	constexpr uint16_t STACK_DEPTH = 2048;
	constexpr uint8_t QUE_LEN = 8;

	struct CTX {
	// Task Context
		std::shared_ptr<ModbusClient> rtu_client;
		SemaphoreHandle_t mutex;
		QueueHandle_t que;
        QueueHandle_t que_rh;
	};

	struct QUE_ELEMENT {
	// Individual measurements
		uint32_t time_ms;
		float data;
	};

	void task(void *param);
	void task_rh(void *param);
	QueueHandle_t create_task(SemaphoreHandle_t mutex_uart, std::shared_ptr<ModbusClient> rtu_client);
    QueueHandle_t create_task_rh(SemaphoreHandle_t mutex_uart, std::shared_ptr<ModbusClient> rut_client);
}

