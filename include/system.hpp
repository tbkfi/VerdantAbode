/* VerdantAbode
 * system.hpp
 *
 * Tuomo Björk
*/
#pragma once

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"


namespace SYSTEM {
	// Variables
	constexpr uint16_t CO2_FLOOR = 200; // Lowest allowed CO2 target
	constexpr uint16_t CO2_CEIL = 1500; // Highest allowed CO2 target
	constexpr uint16_t CO2_INCR = 25;   // Delta per ctrl increment

	// Event flags
	constexpr uint8_t FLAG_CO2_HIGH = ( 1 << 0 );

	struct DATA {
	// System State
		uint32_t co2_target;          // CO2 Level
		EventGroupHandle_t events;    // Event flags
		SemaphoreHandle_t mutex_i2c;  // I2C Lock
		SemaphoreHandle_t mutex_uart; // UART Lock
		QueueHandle_t input_queue;    // Local inputs
		QueueHandle_t sdp610_queue;   // SDP610::PARAM->que
		QueueHandle_t gmp252_queue;   // GMP252::PARAM->que
	};
}
