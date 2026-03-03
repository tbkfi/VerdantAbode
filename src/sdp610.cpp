#include "sdp610.hpp"


void task_sdp610(void* param) {
	SDP610::PARAM *pr = (SDP610::PARAM*) param;

	// System State, and Sensor Queue
	SYSTEM::DATA *sys = pr->sys;
	QueueHandle_t que = pr->que;
	
	uint8_t buffer[3]; 
	uint8_t ctr = 0;
	uint8_t crc = 0;
	int16_t reading = 0;
	int16_t reading_corrected = 0;

	TickType_t last_ran = xTaskGetTickCount();
	TickType_t poll_time = pdMS_TO_TICKS(SDP610::POLL_MS);

	while (true) {
		vTaskDelayUntil(&last_ran, poll_time);

		// Instruct to Measure
		if (xSemaphoreTake(sys->semaphore_i2c, pdMS_TO_TICKS(25)) == pdTRUE) {
			i2c_write_blocking(Pin::I2C1_UNIT, SDP610::ADDR, &SDP610::CMD_MEASURE, 1, false);
			xSemaphoreGive(sys->semaphore_i2c);
			vTaskDelay(pdMS_TO_TICKS(SDP610::INTEGRATION_TIME_MS)); 

			// Read Measurement
			if (xSemaphoreTake(sys->semaphore_i2c, pdMS_TO_TICKS(100)) == pdTRUE) {
				ctr = i2c_read_blocking(Pin::I2C1_UNIT, SDP610::ADDR, buffer, 3, false);
				xSemaphoreGive(sys->semaphore_i2c);

				// Expect 3 bytes: MSB, LSB, CRC
				if (ctr == 3) {
					// Construct reading
					reading = (int16_t) ((buffer[0] << 8) | buffer[1]);

					// Scaling Factor to get Pascals
					// Datasheet: 2. Electrical Specifications
					reading = reading / SDP610::SCALE_FACTOR;

					// TODO: CRC check
					crc = (uint8_t) buffer[2];

					// Datasheet: 5.4 Altitude Correction 
					// >> DP_eff = DP_sensor * (P_cal / P_amb)
					reading_corrected = (uint16_t) reading * SDP610::CORRECTION_FACTOR;

					// Store, Report
					xQueueSend(sys->sdp610_queue, &reading_corrected, 0);
					printf("[%lu] SDP610: %d Pa\n", uptime_ms(), reading_corrected);
				}
			}
		}
	}
}

QueueHandle_t task_create_sdp610(SYSTEM::DATA* system_state) {
	static SDP610::PARAM sdp610_param;
	sdp610_param.sys = system_state;
	sdp610_param.que = xQueueCreate(8, sizeof(SDP610::MEASUREMENT));

	xTaskCreate(task_sdp610, "SDP610", 512, (void *) &sdp610_param, tskIDLE_PRIORITY + 2, NULL);

	return sdp610_param.que;
}
