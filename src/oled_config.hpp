// Raspberry Pi
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
// Keijo
#include "ssd1306.h"
#include "PicoI2CDevice.h"
// FreeRTOS
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
// std library 
#include <stdint.h>
#include <stdio.h>

namespace SSD_1306 {
	inline i2c_inst_t* const I2C_UNIT = i2c1;
	constexpr unsigned ADDR = 0x3C;
	constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 3;
	constexpr uint16_t STACK_DEPTH = 512;
	constexpr uint8_t QUE_LEN = 8;
	constexpr uint32_t I2C_TIMEOUT_US = 10 * 1000;
	constexpr uint8_t INTEGRATION_TIME_MS = 8;


}

void ssd1306_draw_fullscreen(int16_t co2, uint32_t temp, uint32_t press, bool status);
void ssd1306_screen();
void ssd1306_c02(int16_t co2);
void ssd1306_temp(uint32_t temp);
void ssd1306_pressure(uint32_t press); 
void ssd1306_wifi(bool status); 

void task_ssd1306(void *param);
void task_create_ssd1306(SemaphoreHandle_t mutex_i2c);

// check event group bits
