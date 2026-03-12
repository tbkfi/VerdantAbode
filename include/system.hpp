/* VerdantAbode
 * system.hpp
 *
 * Tuomo Björk
*/
#pragma once

#include <stdint.h>
#include <memory>

#include "hardware/i2c.h"

#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include <uart.hpp>
#include <modbus_client.hpp>

#include "ssd1306.h"
#include "PicoI2CBus.h"
#include "PicoI2CDevice.h"


namespace SYSTEM {
	// Variables
	constexpr uint16_t CO2_TARGET = 800;    // Target CO2 level (ppm)
	constexpr uint16_t CO2_SPAN = 15;       // +- tolerance around CO2_TARGET

	constexpr uint16_t CO2_FLOOR = 200;     // Lowest allowed CO2 target
	constexpr uint16_t CO2_CEIL = 1500;     // Highest allowed CO2 target
	constexpr uint16_t CO2_INCR = 25;       // Delta per ctrl increment
	constexpr uint16_t CO2_CRITICAL = 2000; // Critical CO2 limit

	constexpr uint8_t SSID_BUFF_LEN = 128;

	// Event flags
	constexpr EventBits_t FLAG_CO2_HIGH       = ( 1 << 0 );  // Indicates high CO2
	constexpr EventBits_t FLAG_CO2_LOW        = ( 1 << 1 );  // Indicates low CO2
	constexpr EventBits_t FLAG_WIFI_SETUP     = ( 1 << 2 );  // WIFI setup screen?
	constexpr EventBits_t FLAG_WIFI_PFIELD    = ( 1 << 3 );  // Pass-field selected?
	constexpr EventBits_t FLAG_WIFI_CONNECTED = ( 1 << 4 );  // Wifi connetion status
	constexpr EventBits_t FLAG_VALVE_OPEN     = ( 1 << 5 );  // CO2 valve is open

	// Uart & Modbus
	constexpr uint8_t UART_NR = 1;
	constexpr uint8_t UART_TX_PIN = 4;
	constexpr uint8_t UART_RX_PIN = 5;
	constexpr uint16_t BAUD_RATE = 9600;
	constexpr uint8_t STOP_BITS = 2;

	struct DATA {
	// System State
		EventGroupHandle_t events = xEventGroupCreate();

		uint32_t co2_target = SYSTEM::CO2_TARGET;  // Target CO2 level
		char* wifi_ssid[SSID_BUFF_LEN] = {0};      // Wifi SSID
		char* wifi_pass[SSID_BUFF_LEN] = {0};      // Wifi Password
		uint8_t wifi_ssid_pos = 0;
		uint8_t wifi_pass_pos = 0;

		// Sensor & Actuator
		int16_t val_co2  = 0;
		float   val_temp = 0;
		float   val_pa   = 0;
		int     val_fan  = 0;

		TickType_t time_valve_opened_ms = 0; // When valve was prev. opened
		TickType_t time_valve_closed_ms = 0; // When valve was prev. closed

		// Input Related
		TickType_t prev_input_time_ms = 0;  // When last input was
		char setup_c  = 0;                  // Current Character (setup input fields)
		int ctr_input = 0;                  // Auxiliary ctr for smoothing inputs

		SemaphoreHandle_t mutex_i2c  = xSemaphoreCreateMutex();
		SemaphoreHandle_t mutex_uart = xSemaphoreCreateMutex();

		QueueHandle_t input_queue  = nullptr;  // Local inputs
		QueueHandle_t sdp610_queue = nullptr;  // SDP610::PARAM->que
		QueueHandle_t gmp252_queue = nullptr;  // GMP252::PARAM->que
		QueueHandle_t hmp60_queue  = nullptr;  // HMP60::PARAM->que
		QueueHandle_t mio_queue    = nullptr;  // MIO::PARAM->que

		// Devices
		std::shared_ptr<PicoUart>     uart       = nullptr;
		std::shared_ptr<ModbusClient> rtu_client = nullptr;
		
		std::shared_ptr<PicoI2CBus>    i2c_bus = nullptr;
		std::shared_ptr<PicoI2CDevice> i2c_dev = nullptr;
		std::shared_ptr<ssd1306>       display = nullptr;
	};
}

namespace Pin {
	// I2C (Sensirion)
	inline i2c_inst_t* const I2C0_UNIT = i2c0;
	constexpr uint32_t I2C0_BAUD = 100 * 1000;
	constexpr uint8_t I2C0_SDA  = 16;
	constexpr uint8_t I2C0_SCL  = 17;

	// I2C (EEPROM)
	inline i2c_inst_t* const I2C1_UNIT = i2c1;
	constexpr uint32_t I2C1_BAUD = 100 * 1000;
	constexpr uint8_t I2C1_SDA  = 14;
	constexpr uint8_t I2C1_SCL  = 15;
}
