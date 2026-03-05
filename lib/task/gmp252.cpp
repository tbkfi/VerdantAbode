
#include <modbus/client.hpp>
#include <modbus/register.hpp>
#include <task/gmp252.hpp>

QueueHandle_t
task_create_gmp252
(SemaphoreHandle_t mutex_i2c, std::shared_ptr<ModbusClient> rtu_client)
{
	static GMP252::CTX ctx;
	ctx.mutex = mutex_i2c;
	ctx.que = xQueueCreate(GMP252::QUE_LEN, sizeof(GMP252::QUE_ELEMENT));
    ctx.rtu_client = rtu_client;

	// Validation, Registration
	if (ctx.que == NULL)
    {
        // deadlock if quueu is NULL!
		while (true) printf("[GMP252] Queue is NULL!\n");
	}

	vQueueAddToRegistry(ctx.que, "GMP252");
	xTaskCreate(task_gmp252, "GMP252", GMP252::STACK_DEPTH, (void *) &ctx, GMP252::TASK_PRIORITY, NULL);

	return ctx.que;
}

void task_gmp252(void* param)
{
	GMP252::CTX* ctx = (GMP252::CTX*) param;
    ModbusRegister gmp252 (ctx->rtu_client, GMP252::ADDRESS, GMP252::REGISTER::CO2_16);

	int rc = 0;
	uint8_t buffer[3]; 
	uint8_t crc = 0;
	int16_t reading_raw = 0;
	int16_t reading = 0;
	GMP252::QUE_ELEMENT e;

	TickType_t measure_start_time = xTaskGetTickCount();
	TickType_t last_ran = xTaskGetTickCount();
	TickType_t poll_time = pdMS_TO_TICKS(GMP252::POLL_INTERVAL_MS);

    // TODO: warmup procedure

	while (true)
    {

        // stage 1: start the measurement
		vTaskDelayUntil(&last_ran, poll_time);

		// Instruct to Measure
		if (GMP252::DEBUG) printf("[GMP252] Trying to obtain mutex (1/2)...\n");

        // get the mutex
		if (xSemaphoreTake(ctx->mutex, pdMS_TO_TICKS(25)) != pdTRUE)
        {
			if (GMP252::DEBUG) printf("[GMP252] Mutex busy. Skipping measurement.\n");

            vTaskDelay(100);
            
            // return to the start
            break;
		}

        if (GMP252::DEBUG) printf("[GMP252] Instructing to Measure...\n");

        measure_start_time = xTaskGetTickCount();

        // end of stage 1
        xSemaphoreGive(ctx->mutex);
        vTaskDelay(pdMS_TO_TICKS(GMP252::INTEGRATION_TIME_MS)); 

        // stage 2: get the result
        if (GMP252::DEBUG) printf("[GMP252] Trying to obtain mutex (2/2)...\n");
        if (xSemaphoreTake(ctx->mutex, pdMS_TO_TICKS(100)) != pdTRUE)
        {
            if (GMP252::DEBUG) printf("[GMP252] Mutex was not obtained.\n");

            break;
        }

        if (GMP252::DEBUG) printf("[GMP252] mutex obtained. Measuring ...\n");

        reading_raw = (int16_t) gmp252.read();

// TODO: conversions and everything happen right here
        reading = reading_raw;

        xSemaphoreGive(ctx->mutex);

        // Queue element
        e.time_ms = pdTICKS_TO_MS(measure_start_time);
        e.data = reading;

        xQueueSend(ctx->que, &e, 0);
        printf("[%u] GMP252: %u Pa\n", pdTICKS_TO_MS(xTaskGetTickCount()), reading);
    }
}
