/* VerdantAbode
 * oled.cpp
 *
 * Matías Villa-Lemes
*/
#include "oled.hpp"
#include "system.hpp"

void OLED::create_task(SYSTEM::DATA* ctx) {
	xTaskCreate(OLED::task, "SSD1306", OLED::STACK_DEPTH, (void *) ctx, OLED::TASK_PRIO, NULL);
}

void OLED::task(void* param) {
	SYSTEM::DATA* ctx = (SYSTEM::DATA*) param;

	TickType_t last_ran = xTaskGetTickCount();
	TickType_t interval_ms = pdMS_TO_TICKS(OLED::INTERVAL_MS);

	uint32_t flags = 0;
	while (true) {
		vTaskDelayUntil(&last_ran, interval_ms);

		flags = xEventGroupGetBits(ctx->events);
		if (xSemaphoreTake(ctx->mutex_i2c, pdMS_TO_TICKS(50)) != pdTRUE) {
		// Mutex unavailable
			if (OLED::DEBUG) printf("[SSD1306] Couldn't get Mutex (I2C)!\n");
            continue;
		} 
		// Mutex available

        if (OLED::DEBUG) printf("[SSD1306] Screen refresh...\n");

        if (xEventGroupGetBits(ctx->events) & SYSTEM::FLAG_WIFI_SETUP) {
        // SETUP SCREEN
            if (OLED::DEBUG) printf("[SSD1306] view: wifi_setup\n");
            OLED::view_wifi_setup(ctx);
        } else {
        // NORMAL SCREEN
            if (OLED::DEBUG) printf("[SSD1306] view: default\n");
            OLED::view_default(ctx);
        }

/*
        if (ctx->current_view == SYSTEM::VIEW::WIFI_SETUP)
        {
            if (true) printf("[SSD1306] view: wifi_setup\n");
            view_wifi_setup(ctx);
        } else {
        // NORMAL SCREEN
            if (true) printf("[SSD1306] view: default\n");
            view_default(ctx);
        }
*/

        xSemaphoreGive(ctx->mutex_i2c);
	}
}


void OLED::view_default(SYSTEM::DATA* ctx) {
	ctx->display->fill(0);
	OLED::frag_ui();
	OLED::frag_co2(ctx);
	OLED::frag_temp(ctx);
	OLED::frag_pa(ctx);
	OLED::frag_wifi_status(ctx);

	ctx->display->text("VIEW:  MAIN", 0, 48, 1); 
	ctx->display->show();
}

void OLED::view_wifi_setup(SYSTEM::DATA* ctx)
{
//void frag_setup_fields(SYSTEM::DATA* ctx) {
//frag_setup_fields(ctx);
//void frag_setup_c(SYSTEM::DATA* ctx) { //}
//frag_setup_c(ctx);
	ctx->display->fill(0);

    int pointer_position = (ctx->wifi_setup_field == 0) ? 0 : 12;
    ctx->display->text("$", 8 * 4, pointer_position);
	ctx->display->text("SSID :", 0, 0, 1);
	ctx->display->text("PASS :", 0, 12, 1);

    ctx->display->text(SYSTEM::CHARSET::ASCII[ctx->wifi_setup_row], 0, 24, 1);

    if (ctx->SSID.size() > 0)
        ctx->display->text(ctx->SSID.c_str(), 8 * 6, 0, 1);

    if (ctx->PASSWORD.size() > 0)
        ctx->display->text(ctx->PASSWORD.c_str(), 8 * 6, 12, 1);

    // print a row of chars
    ctx->display->text("_", 8 * ctx->wifi_setup_column, 26, 1);// cursor

	ctx->display->text("VIEW:  SETUP", 0, 48, 1); 
	ctx->display->show();
}

void OLED::frag_ui(void) {
// TODO: Prettyfication
	// draw some outlines
	// button borders
	// etc.
}

void OLED::frag_co2(SYSTEM::DATA* ctx) {
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%d / %ld", ctx->val_co2, ctx->co2_target);
	
	ctx->display->text("CO2 :", 0, 0, 1);
	ctx->display->text(buffer, 40, 0, 1);
	//ctx->display->text("ppm", 85, 0, 1);
}

void OLED::frag_temp(SYSTEM::DATA* ctx) {
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%.2f C", ctx->val_temp);
	
	ctx->display->text("Temp:", 0, 12, 1);
	ctx->display->text(buffer, 40, 12, 1);
	//ctx->display->text("C", 85, 12, 1); 
}

void OLED::frag_pa(SYSTEM::DATA* ctx) {
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%.4f", ctx->val_pa); 
	
	ctx->display->text("DP  :", 0, 24, 1);
	ctx->display->text(buffer, 40, 24, 1);
	//ctx->display->text("Pa", 85, 24, 1);
}

void OLED::frag_wifi_status(SYSTEM::DATA* ctx) {
	bool is_connected = (xEventGroupGetBits(ctx->events) & SYSTEM::FLAG_WIFI_CONNECTED);
	
	ctx->display->text("WiFi:", 0, 36, 1);
	ctx->display->text(is_connected ? "CONN." : "DISC.", 40, 36, 1); 
}

void OLED::frag_setup_c(SYSTEM::DATA* ctx) {
}
void OLED::frag_setup_fields(SYSTEM::DATA* ctx) {
	ctx->display->text("SSID :", 0, 0, 1);
	ctx->display->text("PASS :", 0, 12, 1);
}
