#include "oled_config.hpp"
#include "wifi.hpp"

void task_create_ssd1306(SemaphoreHandle_t mutex_i2c){
      // Task 
   xTaskCreate(task_ssd1306, "SSD1306", SSD_1306::STACK_DEPTH,(void *) &mutex_i2c, SSD_1306::TASK_PRIORITY, NULL);
}

void task_ssd1306(void* param) {
	SemaphoreHandle_t mutex = (SemaphoreHandle_t*) param;

	while (true) {

		if (SSD_1306::DEBUG) printf("[SSD1306] Trying to obtain I2C Mutex (1/2)...\n");

		if (xSemaphoreTake(mutex, pdMS_TO_TICKS(25)) != pdTRUE) {
		// Mutex unavailable
			if (SSD_1306::DEBUG) printf("[SSD1306] I2C not free!\n");
		}
		else {
		// Mutex obtained
			if (SSD_1306::DEBUG) {
				printf("[SSD_1306] I2C obtained!\n");
			}

			xSemaphoreGive(mutex);
			vTaskDelay(pdMS_TO_TICKS(SSD_1306::INTEGRATION_TIME_MS)); 

			if (rc == PICO_ERROR_GENERIC || rc == PICO_ERROR_TIMEOUT) {
			// I2C Read: Timeout/Error
				if (SSD_1306::DEBUG) printf("[SSD1306] I2C ERR/TIMEOUT!\n");
			}
			else {
			// I2C Read: Got data
				if (SSD_1306::DEBUG)
					printf("[SSD_1306] Trying to obtain I2C Mutex (2/2)...\n");

				if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
				// Mutex unavailable
					if (SSD_1306::DEBUG) printf("[SSD1306] I2C not free!\n");
				}
				else {
				// Mutex obtained
					if (SSD_1306::DEBUG) {
						printf("[SSD_1306] I2C obtained!\n");
                  ssd1306_draw_fullscreen(val_co2, 22,val_pa, wifi_connection);
					}

					// Expect 3 bytes: MSB, LSB, CRC
					rc = i2c_read_timeout_us(SSD_1306::I2C_UNIT, SSD_1306::ADDR, buffer,
							   3, false, SSD_1306::I2C_TIMEOUT_US);
					xSemaphoreGive(mutex);

					if (rc != 3) {
					// Data: Bad | I2C Error
						if (SSD_1306::DEBUG) printf("[SSD1306] I2C ERR/TIMEOUT!\n");
					}
				}
			}
		}
	}
}
