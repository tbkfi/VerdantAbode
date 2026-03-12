/* VerdantAbode
 * mio.hpp
 *
 * Pavel Shishkin
*/
#pragma once

#include <stdint.h>
#include <memory>
#include <deque>

#include "FreeRTOS.h"
#include "portmacro.h"
#include "projdefs.h"
#include "queue.h"
#include "semphr.h"

#include <modbus_client.hpp>
#include <modbus_register.hpp>


namespace FAN {
	namespace REGISTER {
		constexpr uint16_t SPEED_CTL = 0x0000;
	};
	constexpr uint16_t ADDRESS = 1;
	constexpr uint16_t SPEED_MAX = 1000;
	constexpr uint16_t SPEED_MIN = 0;

	constexpr bool DEBUG = true; // Print debugs?

	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 5;
	constexpr uint16_t STACK_DEPTH = 1024;
	constexpr uint8_t QUE_LEN = 8;

	struct CTX {
	// Task Context
		std::shared_ptr<ModbusClient> rtu_client;
		SemaphoreHandle_t mutex;
		QueueHandle_t que;
		int speed_current = 0;
	};

	struct QUE_ELEMENT {
	// Individual que elements
		uint32_t time_ms;
		int data;
	};

	void task(void *param);
	QueueHandle_t create_task(SemaphoreHandle_t mutex_uart, std::shared_ptr<ModbusClient> rtu_client);

	void set_speed(int speed, QueueHandle_t que);
	//void set_target(QueueHandle_t que, int &speed_current, int target, int steps);
}

