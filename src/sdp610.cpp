/* VerdantAbode
 * sdp610.cpp
 *
 * Tuomo Björk
*/
#include "sdp610.hpp"


QueueHandle_t task_create_sdp610(SemaphoreHandle_t mutex_i2c) {
// Helper to automatically create SDP610 task,
// returns the datapoint queue handle.

	static SDP610::CTX ctx;
	ctx.mutex = mutex_i2c;
	ctx.que = xQueueCreate(SDP610::QUE_LEN, sizeof(SDP610::QUE_ELEMENT));

	// Validation, Registration
	if (ctx.que == NULL) {
		while (true) printf("[SDP610] Queue is NULL!!!\n");
	}
	vQueueAddToRegistry(ctx.que, "SDP610");

	// Task
	xTaskCreate(task_sdp610, "SDP610", SDP610::STACK_DEPTH, (void *) &ctx, SDP610::TASK_PRIORITY, NULL);
	return ctx.que;
}

void task_sdp610(void* param) {
	SDP610::CTX* ctx = (SDP610::CTX*) param;

	int rc = 0;
	uint8_t buffer[3]; 
	uint8_t crc = 0;
	int16_t reading_raw = 0;
	float   reading = 0;
	SDP610::QUE_ELEMENT e;

	TickType_t measure_start_time = xTaskGetTickCount();
	TickType_t last_ran = xTaskGetTickCount();
	TickType_t poll_time = pdMS_TO_TICKS(SDP610::POLL_INTERVAL_MS);

	while (true) {
		vTaskDelayUntil(&last_ran, poll_time);

		if (SDP610::DEBUG) printf("[SDP610] Trying to obtain I2C Mutex (1/2)...\n");

		if (xSemaphoreTake(ctx->mutex, pdMS_TO_TICKS(25)) != pdTRUE) {
		// Mutex unavailable
			if (SDP610::DEBUG) printf("[SDP610] I2C not free!\n");
		}
		else {
		// Mutex obtained
			if (SDP610::DEBUG) {
				printf("[SDP610] I2C obtained!\n");
				printf("[SDP610] Instructing to Measure...\n");
			}

			measure_start_time = xTaskGetTickCount();
			rc = i2c_write_timeout_us(SDP610::I2C_UNIT, SDP610::ADDR, &SDP610::CMD_MEASURE,
							 1, false, SDP610::I2C_TIMEOUT_US);
			xSemaphoreGive(ctx->mutex);
			vTaskDelay(pdMS_TO_TICKS(SDP610::INTEGRATION_TIME_MS)); 

			if (rc == PICO_ERROR_GENERIC || rc == PICO_ERROR_TIMEOUT) {
			// I2C Read: Timeout/Error
				if (SDP610::DEBUG) printf("[SDP610] I2C ERR/TIMEOUT!\n");
			}
			else {
			// I2C Read: Got data
				if (SDP610::DEBUG) printf("[SDP610] Trying to obtain I2C Mutex (2/2)...\n");

				if (xSemaphoreTake(ctx->mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
				// Mutex unavailable
					if (SDP610::DEBUG) printf("[SDP610] I2C not free!\n");
				}
				else {
				// Mutex obtained
					if (SDP610::DEBUG) {
						printf("[SDP610] I2C obtained!\n");
						printf("[SDP610] Reading Measurement...\n");
					}

					// Expect 3 bytes: MSB, LSB, CRC
					rc = i2c_read_timeout_us(SDP610::I2C_UNIT, SDP610::ADDR, buffer,
							   3, false, SDP610::I2C_TIMEOUT_US);
					xSemaphoreGive(ctx->mutex);

					if (rc != 3) {
					// Data: Bad | I2C Error
						if (SDP610::DEBUG) printf("[SDP610] I2C ERR/TIMEOUT!\n");
					}
					else {
					// Data: Received expected bytes

						// Construct reading
						reading_raw = (int16_t) ((buffer[0] << 8) | buffer[1]);
						if (SDP610::DEBUG) printf("[SDP610] -> [%02x %02x %02x]\n",
								   buffer[0], buffer[1], buffer[2]);

						// Scaling Factor to get Pascals
						// Datasheet: 2. Electrical Specifications
						reading = (float) reading_raw / SDP610::SCALE_FACTOR;

						// CRC check (TODO)
						crc = (uint8_t) buffer[2];

						// Differential Pressure Altitude Correction (Datasheet: 5.4)
						// >> DP_eff = DP_sensor * (P_cal / P_amb)
						reading = reading * SDP610::CORRECTION_FACTOR;

						// Queue element
						e.time_ms = pdTICKS_TO_MS(measure_start_time);
						e.data = reading;
						xQueueSend(ctx->que, &e, 0);
						if (SDP610::DEBUG) printf("[%lu] SDP610: %f Pa\n", pdTICKS_TO_MS(xTaskGetTickCount()), reading);
					}
				}
			}
		}
	}
}
