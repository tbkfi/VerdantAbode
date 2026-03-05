#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"


namespace GMP252 {
	constexpr bool DEBUG = false; // Print debugs?

	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 2;
	constexpr uint16_t STACK_DEPTH = 1024;
	constexpr uint8_t QUE_LEN = 8;
	constexpr uint8_t INTEGRATION_TIME_MS = 0;

	struct CTX {
	// Task Context
		QueueHandle_t que;
		SemaphoreHandle_t mutex;
	};

	struct QUE_ELEMENT {
	// Individual measurements
		uint32_t time_ms;
		int16_t data;
	};
}
