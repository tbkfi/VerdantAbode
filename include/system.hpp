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
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include <uart.hpp>
#include <modbus_client.hpp>

#include "ssd1306.h"
#include "PicoI2CBus.h"
#include "PicoI2CDevice.h"
#include "mio.hpp"

#define DBG_PRINT(flag, module, ...) \
    do { \
        if (flag) { \
            printf("[%s] ", module); \
            printf(__VA_ARGS__); \
            printf("\n"); \
        } \
    } while (0);

namespace SYSTEM {
	// Variables
	constexpr uint16_t CO2_TARGET = 800;    // Target CO2 level (ppm)
	constexpr uint16_t CO2_SPAN = 25;       // +- tolerance around CO2_TARGET

	constexpr uint16_t CO2_FLOOR = 200;     // Lowest allowed CO2 target
	constexpr uint16_t CO2_CEIL = 1500;     // Highest allowed CO2 target
	constexpr uint16_t CO2_INCR = 25;       // Delta per ctrl increment
	constexpr uint16_t CO2_CRITICAL = 2000; // Critical CO2 limit

	constexpr uint8_t SSID_BUFF_LEN = 128;

	// Event flags
	constexpr uint32_t FLAG_CO2_HIGH       = ( 1 << 0 ); // Indicates high CO2
	constexpr uint32_t FLAG_CO2_LOW        = ( 1 << 1 ); // Indicates low CO2
	constexpr uint32_t FLAG_WIFI_SETUP     = ( 1 << 2 ); // WIFI setup screen?
	constexpr uint32_t FLAG_WIFI_PFIELD    = ( 1 << 3 ); // Pass-field selected?
	constexpr uint32_t FLAG_WIFI_CONNECTED = ( 1 << 4 ); // Wifi connetion status
	constexpr uint32_t FLAG_VALVE_OPEN     = ( 1 << 5 ); // CO2 valve is open
    constexpr uint32_t FLAG_GOTO_MAIN = ( 1 << 6 );

	struct DATA {
	// System State
		uint32_t co2_target;            // Target CO2 level
		char* wifi_ssid[SSID_BUFF_LEN]; // Wifi SSID
		char* wifi_pass[SSID_BUFF_LEN]; // Wifi Password
		uint8_t wifi_ssid_pos = 0;
		uint8_t wifi_pass_pos = 0;
		EventGroupHandle_t events;      // Event flags
		SemaphoreHandle_t mutex_i2c;    // I2C Lock
		SemaphoreHandle_t mutex_uart;   // UART Lock
		QueueHandle_t input_queue;      // Local inputs
		QueueHandle_t sdp610_queue;     // SDP610::PARAM->que
		QueueHandle_t gmp252_queue;     // GMP252::PARAM->que
		QueueHandle_t hmp60_queue;      // HMP60::PARAM->que
		QueueHandle_t mio_queue;        // MIO::PARAM->que

		// Latest sensor values
		int16_t  val_co2;
		float val_temp;
		float val_pa;

		uint16_t val_fan;
    
		// Devices
		std::shared_ptr<PicoUart> uart;
		std::shared_ptr<ModbusClient> rtu_client;
		
		std::shared_ptr<PicoI2CBus> i2c_bus;
		std::shared_ptr<PicoI2CDevice> i2c_dev;
		std::shared_ptr<ssd1306> display;

		// Input Related
		char setup_c;   // Current Character (setup input fields)
        int wifi_setup_row = 0;
        int wifi_setup_column = 0;
        int wifi_setup_field = 0; // 0 for SSID and 1 for password
		int ctr_input;  // Auxiliary ctr for smoothing inputs
    
        std::string SSID;
        std::string PASSWORD;
        
        unsigned current_view = 0; // default to MAIN view
	};
    // usage SYSTEM::VIEW::WIFI_SETUP
    namespace VIEW
    {
        const unsigned MAIN = 0;
        const unsigned WIFI_SETUP = 1;
        const unsigned WIFI_CONNECTION = 2;
    }

    namespace CHARSET {
        constexpr const char* CHAR_ROW_ALPHA_LOWER_0 =
            "abcdefghijklmnop";
        constexpr const char* CHAR_ROW_ALPHA_LOWER_1 =
            "qrstuvwxyz";

        constexpr const char* CHAR_ROW_ALPHA_UPPER_0 =
            "ABCDEFGHIJKLMNOP";
        constexpr const char* CHAR_ROW_ALPHA_UPPER_1 =
            "QRSTUVWXYZ";
        
        constexpr const char* CHAR_ROW_SPECIAL_0 =
            "0123456789_ +!#$%";
        constexpr const char* CHAR_ROW_SPECIAL_1 =
            "^&()[]<>= /\\";

        constexpr const char* CHAR_ROW_CONTROL =
            "<%N+ SPECIAL CHARACTERS";

        constexpr const char* ASCII[] = {
            CHAR_ROW_ALPHA_LOWER_0,
            CHAR_ROW_ALPHA_LOWER_1,
            CHAR_ROW_ALPHA_UPPER_0,
            CHAR_ROW_ALPHA_UPPER_1,
            CHAR_ROW_SPECIAL_0,
            CHAR_ROW_SPECIAL_1,
            CHAR_ROW_CONTROL
        };
        constexpr const int ROW_COUNT = 7;
    }

	// Modbus
	constexpr uint8_t UART_NR = 1;
	constexpr uint8_t UART_TX_PIN = 4;
	constexpr uint8_t UART_RX_PIN = 5;
	constexpr uint16_t BAUD_RATE = 9600;
	constexpr uint8_t STOP_BITS = 2;
}

namespace Pin {
	// Local Vent
	constexpr uint8_t EXHAUST_VALVE = 32;

	// UART (Simulator)
	constexpr uint8_t UART_TX_SLAVE = 5;
	constexpr uint8_t UART_RX_SLAVE = 4;

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
