#include "oled.hpp"

void task_create_ssd1306(SYSTEM::DATA* ctx) {
	xTaskCreate(task_ssd1306, "SSD1306", OLED::STACK_DEPTH, (void *) ctx, OLED::TASK_PRIORITY, NULL);
}

void task_ssd1306(void* param) {
	SYSTEM::DATA* ctx = (SYSTEM::DATA*) param;

	TickType_t last_ran = xTaskGetTickCount();
	TickType_t interval_ms = pdMS_TO_TICKS(OLED::INTERVAL_MS);

	uint32_t flags = 0;
	while (true) {
		vTaskDelayUntil(&last_ran, interval_ms);

		// DEMO VALUES
		int16_t current_co2 = 400;
		uint32_t current_temp = 22;
		uint32_t current_pa = 101325;
		bool is_wifi_connected = true;
		// DEMO VALUES

		flags = xEventGroupGetBits(ctx->events);
		if (xSemaphoreTake(ctx->mutex_i2c, pdMS_TO_TICKS(50)) != pdTRUE) {
		// Mutex unavailable
			if (OLED::DEBUG) printf("[SSD1306] Couldn't get Mutex (I2C)!\n");
		} else {
		// Mutex available
			if (OLED::DEBUG) printf("[SSD1306] Screen refresh...\n");

			if (xEventGroupGetBits(ctx->events) & SYSTEM::FLAG_WIFI_SETUP) {
			// SETUP SCREEN
				// draw wifi_setup screen
			} else {
			// NORMAL SCREEN
				frag_ui();
				frag_co2(ctx->val_co2);
				frag_temp(ctx->val_temp);
				frag_pa(ctx->val_pa);
				frag_wifi_status(false);
				//display.show(); 
			}
			xSemaphoreGive(ctx->mutex_i2c);
		}
	}
}

void ssd1306_screen() {
	//display.fill(0);
	//display.text("[Co2]: ", 0, 0, 1);
	//display.text("[Temp]: ", 0, 13, 1);
	//display.text("[P]: ", 0, 26, 1);
	//display.text("[WiFi]: ", 0, 39, 1);
	//display.text("[A <- -> B]", 0, 52, 1);
}

void frag_ui(void) {
}

void frag_co2(int16_t val) {
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%d", val);
	//display.text(buffer, 50, 0, 1);
	//display.text("ppm", 70, 0, 1);
}

void frag_temp(uint32_t val) {
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%lu", val);
	//display.text(buffer, 50, 13, 1);
	//display.text("C", 70, 13, 1);
}

void frag_pa(uint32_t val) {
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%lu", val);
	//display.text(buffer, 50, 26, 1);
	//display.text("Pa", 70, 26, 1);
}

void frag_wifi_status(bool stat) {
	//display.text(stat ? "ON" : "OFF", 50, 39, 1); 
}
