#pragma once

#include <stdint.h>
#include <memory>

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include <modbus_client.hpp>
#include <modbus_register.hpp>


void task_mio(void *param);
QueueHandle_t task_create_mio
(SemaphoreHandle_t mutex_, std::shared_ptr<ModbusClient> rtu_client);

namespace FAN {
    namespace REGISTER {
        constexpr uint16_t SPEED_CTL = 0x0000;
    };
    constexpr uint16_t ADDRESS = 1;

	constexpr bool DEBUG = false; // Print debugs?
	constexpr uint16_t POLL_INTERVAL_MS = 1 * 1000;

	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 2;
	constexpr uint16_t STACK_DEPTH = 2048;
	constexpr uint8_t QUE_LEN = 8;

	typedef struct {
	// Task Context
        std::shared_ptr<ModbusClient> rtu_client;
		SemaphoreHandle_t mutex;
		QueueHandle_t que;
	} CTX;

    typedef struct {
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
