/* VerdantAbode
 * mio.cpp
 *
 * Pavel Shishkin
*/
#include "mio.hpp"


QueueHandle_t task_create_mio(SemaphoreHandle_t mutex_uart, std::shared_ptr<ModbusClient> rtu_client) {
	static FAN::CTX ctx;
	ctx.mutex = mutex_uart;
	ctx.rtu_client = rtu_client;
	ctx.que = xQueueCreate(FAN::QUE_LEN, sizeof(FAN::QUE_ELEMENT));

	// Validate, Register
	if (ctx.que == NULL) {
		while (true) printf("[FAN] Que is NULL!\n");
	}
	vQueueAddToRegistry(ctx.que, "MIO");

	xTaskCreate(task_mio, "FAN_CTL", FAN::STACK_DEPTH, (void*) &ctx, FAN::TASK_PRIORITY, NULL);
	return ctx.que;
}

void task_mio(void *param) {
	FAN::CTX* ctx = (FAN::CTX*) param;
	ModbusRegister fan(ctx->rtu_client, FAN::ADDRESS, FAN::REGISTER::SPEED_CTL);

	FAN::QUE_ELEMENT e;
	while (true) {
	// The fan speed is updated via e->data,
	// whenever an element is received.
		if (xQueueReceive(ctx->que, &e, portMAX_DELAY) == pdTRUE) {
			if (FAN::DEBUG) printf("[FAN] (status): updating to %d.\n", e.data);

			if (xSemaphoreTake(ctx->mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
				fan.write(e.data);
				xSemaphoreGive(ctx->mutex);
			} else {
				if (FAN::DEBUG) printf("[FAN] (mutex): unable to obtain.\n");
			}
		}
	}
}

void fan_set_speed(int speed_percentage, QueueHandle_t que) {
	FAN::QUE_ELEMENT e;
	int one_pr = FAN::SPEED_MAX / 100;
	e.time_ms = pdTICKS_TO_MS(xTaskGetTickCount());

	e.data = speed_percentage * one_pr;
	if (e.data < FAN::SPEED_MIN) e.data = FAN::SPEED_MIN;

	xQueueSend(que, &e, pdMS_TO_TICKS(100));
}


/*
void fan_speed_target (void* ctx, int target, int steps) {
    SYSTEM::DATA* context = (SYSTEM::DATA*) ctx;

    // LAST_SPEED -> to, using # of steps
    int delta = target - context->val_fan;
    int step = delta / steps;

    if (FAN::DEBUG) printf("[FAN] Appending to the queue failed! \n");

    for (int i = context->val_fan; i != target; i + step)
    {
        if (pdPASS != xQueueSend(context->mio_queue, &i, pdMS_TO_TICKS(100)))
            if (FAN::DEBUG) printf("[FAN] Appending to the queue failed! \n");
    }

    context->val_fan = target;
}
*/
