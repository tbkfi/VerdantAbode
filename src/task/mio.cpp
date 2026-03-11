#include "mio.hpp"

QueueHandle_t task_create_mio
(SemaphoreHandle_t mutex_uart, std::shared_ptr<ModbusClient> rtu_client)
{
    static FAN::CTX ctx;
    ctx.mutex = mutex_uart;
    ctx.rtu_client = rtu_client;
    ctx.que = xQueueCreate(FAN::QUE_LEN, sizeof(FAN::QUE_ELEMENT));

    if (ctx.que == NULL) {
        while (true) printf("[FAN] Que is NULL!\n");
    }
    vQueueAddToRegistry(ctx.que, "FAN_CTL");

    xTaskCreate(task_mio, "FAN_CTL", FAN::STACK_DEPTH, (void*) &ctx, FAN::TASK_PRIORITY, NULL);

    return ctx.que;
}

void task_mio(void *param) {
    FAN::CTX* ctx = (FAN::CTX*) param;
    ModbusRegister fan (ctx->rtu_client, FAN::ADDRESS, FAN::REGISTER::SPEED_CTL);

    TickType_t last_ran = xTaskGetTickCount();
    TickType_t interval_ms = pdMS_TO_TICKS(FAN::POLL_INTERVAL_MS);

    while (true) {
        vTaskDelayUntil(&last_ran, interval_ms);
        FAN::QUE_ELEMENT speed;

        // check if there are changes to fan speed
        if (xQueueReceive(ctx->que, &speed, pdMS_TO_TICKS(100) != pdTRUE)) {
            if (FAN::DEBUG) printf("[FAN] No changes to fan speed. \n");
            continue;
        }

        // if any, take the mutex and start changing speed
        if (xSemaphoreTake(ctx->mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            if (FAN::DEBUG) printf("[FAN] Mutex busy. Skipping this cycle. \n");
            continue;
        }
        
        if (FAN::DEBUG) printf("[FAN] New speed: %d. \n", speed);
        fan.write(speed.data);
        
        xSemaphoreGive(ctx->mutex);
    }
}
