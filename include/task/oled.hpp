// Raspberry Pi
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
// Keijo
#include "ssd1306/ssd1306.h"
#include "PicoI2CDevice.h"
// FreeRTOS
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
// std library 
#include <stdint.h>
#include <stdio.h>

#include "system.hpp"


namespace OLED {
	inline i2c_inst_t* const I2C_UNIT = i2c1;
	constexpr uint8_t BUS_NR = 1;
	constexpr unsigned ADDR = 0x3C;
	constexpr uint8_t PIN_SCL = 15;
	constexpr uint8_t PIN_SDA = 14;

	constexpr bool DEBUG = true; // print debugs?

	constexpr uint16_t INTERVAL_MS = 500;
	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 1;
	constexpr uint16_t STACK_DEPTH = 1024;
	constexpr uint32_t I2C_TIMEOUT_US = 10 * 1000;
}

void ssd1306_screen();
void frag_ui(void);
void frag_co2(int16_t val);
void frag_temp(uint32_t val);
void frag_pa(uint32_t val); 
void frag_wifi_status(bool stat); 

void task_ssd1306(void *param);
void task_create_ssd1306(SYSTEM::DATA* ctx);
