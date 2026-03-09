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

#include <modbus_client.hpp>
#include <modbus_register.hpp>


void task_gmp252(void *param);
QueueHandle_t
task_create_gmp252
(SemaphoreHandle_t mutex_i2c, std::shared_ptr<ModbusClient> rtu_client);

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
	constexpr uint16_t POLL_INTERVAL_MS = 1 * 1000;

	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 2;
	constexpr uint16_t STACK_DEPTH = 2048;
	constexpr uint8_t QUE_LEN = 8;
	constexpr uint16_t INTEGRATION_TIME_MS = 12 * 1000; // Typical: lt 12s --> 2min

	typedef struct {
	// Task Context
        std::shared_ptr<ModbusClient> rtu_client;
		SemaphoreHandle_t mutex;
		QueueHandle_t que;
	} CTX;

	typedef struct  {
	// Individual measurements
		uint32_t time_ms;
		int16_t data;
	} QUE_ELEMENT;

    // NOTE: implement(maybe).
    #if 0
    #define DBG_PRINT(...) \
        do { \
            if (debug::enabled) { \
                printf("TYPE: "); \
                printf(__VA_ARGS__); \
            } \
        } while(0)

    void dbg_print(__VA_ARGS__)
    {
        if (DEBUG)
            printf("[GMP252]");
            printf(__VA_ARGS__);
    }
    #endif
}
