/* VerdantAbode
 * system.hpp
 *
 * Tuomo Björk
*/
#pragma once

#include <stdint.h>
#include <memory>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include <modbus/client.hpp>
#include <modbus/register.hpp>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"


#define DBG_PRINT(flag, module ,...) \
    do { \
        if (flag) { \
            printf("[%s] ", module); \
            printf(__VA_ARGS__); \
            printf("\n"); \
        } \
    } while (0);
    

namespace SYSTEM {
    constexpr unsigned SENSOR_ARR_LENGTH = 24;
    namespace I2C {
        constexpr unsigned INSTANCE_DEFAULT = 1;
        constexpr unsigned SDA_DEFAULT = 14;
        constexpr unsigned SCL_DEFAULT = 15;
        // I2C Addr.
    }
    namespace UART {
        constexpr unsigned NR_DEFAULT = 1;
        constexpr unsigned BAUD_RATE_DEFAULT = 9600;
        constexpr unsigned TX_PIN_DEFAULT = 4;
        constexpr unsigned RX_PIN_DEFAULT = 5;
        constexpr unsigned STOP_BITS_DEFAULT = 2;
    }
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
        std::shared_ptr<ModbusClient> rtu_client; // modbus client
	};

    namespace PIN {
	// Local Inputs
	const unsigned char BTN1 = 7;
	const unsigned char BTN2 = 8;
	const unsigned char BTN3 = 9;

	const unsigned char ROTA = 10;
	const unsigned char ROTB = 11;
	const unsigned char ROTC = 12;

	// Local Vent
	const unsigned char EXHAUST_VALVE = 32;

	// Local LED indicators
	const unsigned char LED1 = 20;
	const unsigned char LED2 = 21;
	const unsigned char LED3 = 22;
	
	// UART (Main)
	const unsigned char UART_TX_MASTER = 4;
	const unsigned char UART_RX_MASTER = 5;

	// UART (Simulator)
	const unsigned char UART_TX_SLAVE = 5;
	const unsigned char UART_RX_SLAVE = 4;

	// I2C (Sensirion)
	inline i2c_inst_t* const I2C0_UNIT = i2c0;
	const uint32_t      I2C0_BAUD = 100 * 1000;
	const unsigned char I2C0_SDA  = 16;
	const unsigned char I2C0_SCL  = 17;

	// I2C (EEPROM)
	inline i2c_inst_t* const I2C1_UNIT = i2c1;
	const uint32_t      I2C1_BAUD = 100 * 1000;
	const unsigned char I2C1_SDA  = 14;
	const unsigned char I2C1_SCL  = 15;
}
}
