<<<<<<< HEAD
=======
/* VerdantAbode
 * mio.hpp
 *
 * Pavel Shishkin
*/
>>>>>>> 3acc3dd134f39ec9f7625cab900d9cd7c4f1a684
#pragma once

#include <stdint.h>
#include <memory>
#include <deque>

#include "FreeRTOS.h"
<<<<<<< HEAD
=======
#include "portmacro.h"
#include "projdefs.h"
>>>>>>> 3acc3dd134f39ec9f7625cab900d9cd7c4f1a684
#include "queue.h"
#include "semphr.h"

#include <modbus_client.hpp>
#include <modbus_register.hpp>

<<<<<<< HEAD
#include "system.hpp"

namespace FAN {
    namespace REGISTER {
        constexpr uint16_t SPEED_CTL = 0x0000;
    };
    constexpr uint16_t ADDRESS = 1;

	constexpr bool DEBUG = true; // Print debugs?
	constexpr uint16_t POLL_INTERVAL_MS = 1 * 1000;

	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 10;
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

    void dbg_print(__VA_ARGS__) {
        if (DEBUG)
            printf("[GMP252]");
            printf(__VA_ARGS__);
    }
    #endif
}

void task_mio(void *param);
QueueHandle_t task_create_mio
(SemaphoreHandle_t mutex_uart, std::shared_ptr<ModbusClient> rtu_client);

void fan_speed_target(void* ctx, int target, int steps);
=======

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

>>>>>>> 3acc3dd134f39ec9f7625cab900d9cd7c4f1a684
