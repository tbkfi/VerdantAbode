/* VerdantAbode
 * oled.hpp
 *
 * Matías Villa-Lemes
*/
#pragma once

#include <stdint.h>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

#include "ssd1306/ssd1306.h"
#include "PicoI2CDevice.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "system.hpp"


namespace OLED {
	inline i2c_inst_t* const I2C_UNIT = i2c1;
	constexpr uint8_t BUS_NR = 1;
	constexpr unsigned ADDR = 0x3C;
	constexpr uint8_t PIN_SCL = 15;
	constexpr uint8_t PIN_SDA = 14;

	constexpr bool DEBUG = true; // Print debugs?

	constexpr uint8_t TASK_PRIO = tskIDLE_PRIORITY + 10;
	constexpr uint16_t INTERVAL_MS = 250;
	constexpr uint16_t STACK_DEPTH = 1024;
	constexpr uint32_t I2C_TIMEOUT_US = 10 * 1000;

	// Task
	void task(void *param);
	void create_task(SYSTEM::DATA* ctx);

	// Complete views
	void view_default(SYSTEM::DATA* ctx);
	void view_wifi_setup(SYSTEM::DATA* ctx);

	// Fragments
	void frag_ui(void);
	void frag_co2(SYSTEM::DATA* ctx);
	void frag_temp(SYSTEM::DATA* ctx);
	void frag_pa(SYSTEM::DATA* ctx);
	void frag_rh(SYSTEM::DATA* ctx);
	void frag_wifi_status(SYSTEM::DATA* ctx);

	void frag_setup_c(SYSTEM::DATA* ctx);
	void frag_setup_fields(SYSTEM::DATA* ctx);
}
