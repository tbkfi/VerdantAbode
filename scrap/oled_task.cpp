#include "oled_config.hpp"

QueueHandle_t task_create_ssd1306(SemaphoreHandle_t mutex_i2c){
   
   static SSD_1306::CTX ctx;
   ctx.mutex = mutex_i2c;
   ctx.que = xQueueCreate(SSD_1306::QUE_LEN, sizeof(SSD_1306::QUE_ELEMENT));

   // Validation, Registration 

   if (ctx.que == NULL){
      while (true) printf("[SSD1306] Queue is NULL!!!\n");
   }
   vQueueAddToRegistry(ctx.que, "SSD1306");

   // Task 
   xTaskCreate(task_ssd1306, "SSD1306", SSD_1306::STACK_DEPTH, (void *) &ctx, SSD_1306::TASK_PRIORITY, NULL);
   return ctx.que;
}

void task_ssd1306(void* param) {
	SSD_1306::CTX* ctx = (SSD_1306::CTX*) param;

	int rc = 0;
	uint8_t buffer[3]; 
	uint8_t crc = 0;
	SSD_1306::QUE_ELEMENT e;

	while (true) {

		if (SSD_1306::DEBUG) printf("[SSD1306] Trying to obtain I2C Mutex (1/2)...\n");

		if (xSemaphoreTake(ctx->mutex, pdMS_TO_TICKS(25)) != pdTRUE) {
		// Mutex unavailable
			if (SSD_1306::DEBUG) printf("[SSD1306] I2C not free!\n");
		}
		else {
		// Mutex obtained
			if (SSD_1306::DEBUG) {
				printf("[SSD_1306] I2C obtained!\n");
			}

			xSemaphoreGive(ctx->mutex);
			vTaskDelay(pdMS_TO_TICKS(SSD_1306::INTEGRATION_TIME_MS)); 

			if (rc == PICO_ERROR_GENERIC || rc == PICO_ERROR_TIMEOUT) {
			// I2C Read: Timeout/Error
				if (SSD_1306::DEBUG) printf("[SSD1306] I2C ERR/TIMEOUT!\n");
			}
			else {
			// I2C Read: Got data
				if (SSD_1306::DEBUG)
					printf("[SSD_1306] Trying to obtain I2C Mutex (2/2)...\n");

				if (xSemaphoreTake(ctx->mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
				// Mutex unavailable
					if (SSD_1306::DEBUG) printf("[SSD1306] I2C not free!\n");
				}
				else {
				// Mutex obtained
					if (SSD_1306::DEBUG) {
						printf("[SSD_1306] I2C obtained!\n");
					}

					// Expect 3 bytes: MSB, LSB, CRC
					rc = i2c_read_timeout_us(SSD_1306::I2C_UNIT, SSD_1306::ADDR, buffer,
							   3, false, SSD_1306::I2C_TIMEOUT_US);
					xSemaphoreGive(ctx->mutex);

					if (rc != 3) {
					// Data: Bad | I2C Error
						if (SSD_1306::DEBUG) printf("[SSD1306] I2C ERR/TIMEOUT!\n");
					}
				}
			}
		}
	}
}
