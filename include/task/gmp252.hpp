/* VerdantAbode
 * gmp252.hpp
 *
 * Pavel Shishkin
*/
#pragma once

#include <stdint.h>
#include <memory>

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "uart.hpp"
#include "modbus_client.hpp"
#include "modbus_register.hpp"


namespace GMP252 {
    namespace REGISTER {
        // https://www.fondriest.com/pdf/vaisala_gmp252_manual.pdf, table 56

        constexpr uint16_t CO2_32F = 0x0000;    // ppm as 32bit float
        constexpr uint16_t CO2_16 = 0x0100;     // ppm as 16bit int up to 32 000 ppm
        constexpr uint16_t CO2_16E10 = 0x0101;  // ppm as 16bit int scaled * 10
        constexpr uint16_t T_32F_COMPENSATION = 0x0002;
        constexpr uint16_t T_32F_MEASURED = 0x0004;
    };
    constexpr uint16_t ADDRESS = 240;

	constexpr bool DEBUG = false; // Print debugs?
	constexpr uint16_t POLL_INTERVAL_MS = 200;

	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 3;
	constexpr uint16_t STACK_DEPTH = 2048;
	constexpr uint8_t QUE_LEN = 8;
	constexpr uint16_t INTEGRATION_TIME_MS = 12 * 1000; // Typical: lt 12s --> 2min

	typedef struct {
	// Task Context
        std::shared_ptr<ModbusClient> rtu_client;
		std::shared_ptr<PicoUart> uart;
		SemaphoreHandle_t mutex;
		QueueHandle_t que;
	} CTX;

	typedef struct {
	// Individual measurements
		uint32_t time_ms;
		int16_t data;
	} QUE_ELEMENT;

	void task(void *param);
	QueueHandle_t create_task (SemaphoreHandle_t mutex_uart,
				std::shared_ptr<ModbusClient> rtu_client, std::shared_ptr<PicoUart> uart);
}
