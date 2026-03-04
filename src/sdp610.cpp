#include "sdp610.hpp"


QueueHandle_t task_create_sdp610(SemaphoreHandle_t mutex_i2c) {
	static SDP610::CTX ctx;
	ctx.sem = mutex_i2c;
	ctx.que = xQueueCreate(SDP610::QUE_LEN, sizeof(SDP610::MEASUREMENT));

	// Validation, Registration
	if (ctx.que == NULL) {
		while (true) printf("[SDP610] Queue is NULL!\n");
	}
	vQueueAddToRegistry(ctx.que, "SDP610");

	xTaskCreate(task_sdp610, "SDP610", SDP610::STACK_DEPTH, (void *) &ctx, SDP610::TASK_PRIORITY, NULL);

	return ctx.que;
}

void task_sdp610(void* param) {
	SDP610::CTX* ctx = (SDP610::CTX*) param;

	uint8_t buffer[3]; 
	uint8_t ctr = 0;
	uint8_t crc = 0;
	int16_t reading_raw = 0;
	float   reading = 0;

	TickType_t last_ran = xTaskGetTickCount();
	TickType_t poll_time = pdMS_TO_TICKS(SDP610::POLL_INTERVAL_MS);

	while (true) {
		vTaskDelayUntil(&last_ran, poll_time);

		// Instruct to Measure
		if (SDP610::DEBUG) printf("[SDP610] Trying to obtain I2C Semaphore (1/2)...\n");
		if (xSemaphoreTake(ctx->sem, pdMS_TO_TICKS(25)) != pdTRUE) {
			if (SDP610::DEBUG) printf("[SDP610] I2C not free!\n");
		}
		else {
			if (SDP610::DEBUG) printf("[SDP610] Instructing to Measure...\n");
			i2c_write_blocking(Pin::I2C1_UNIT, SDP610::ADDR, &SDP610::CMD_MEASURE, 1, false);
			xSemaphoreGive(ctx->sem);
			vTaskDelay(pdMS_TO_TICKS(SDP610::INTEGRATION_TIME_MS)); 

			// Read Measurement
			if (SDP610::DEBUG) printf("[SDP610] Trying to obtain I2C Semaphore (2/2)...\n");
			if (xSemaphoreTake(ctx->sem, pdMS_TO_TICKS(100)) == pdTRUE) {
				if (SDP610::DEBUG) printf("[SDP610] Reading Measurement...\n");
				ctr = i2c_read_blocking(Pin::I2C1_UNIT, SDP610::ADDR, buffer, 3, false);
				xSemaphoreGive(ctx->sem);

				// Expect 3 bytes: MSB, LSB, CRC
				if (ctr == 3) {
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

					// Store, Report
					xQueueSend(ctx->que, &reading, 0);
					printf("[%lu] SDP610: %f Pa\n", pdTICKS_TO_MS(xTaskGetTickCount()), reading);
				}
			}
		}
	}
}
