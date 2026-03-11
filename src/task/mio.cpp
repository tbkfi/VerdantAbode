#include "mio.hpp"

void fan_speed_target
(void* ctx, int target, int steps)
{
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
    ModbusRegister fan(ctx->rtu_client, FAN::ADDRESS, FAN::REGISTER::SPEED_CTL);

    std::deque<int> transition;

    TickType_t last_ran = xTaskGetTickCount();
    TickType_t interval_ms = pdMS_TO_TICKS(FAN::POLL_INTERVAL_MS);

    while (true) {
        vTaskDelayUntil(&last_ran, interval_ms);
        FAN::QUE_ELEMENT speed;

        if (xQueueReceive(ctx->que, &speed, 0) == pdTRUE)
        {
            transition.push_back(speed.data);
        }
        else if (transition.empty())
        {
            if (FAN::DEBUG) printf("[FAN] No changes to fan speed. \n");
            continue;
        }
        // if we got here, the speed will be changing

        // if any, take the mutex and start changing speed
        if (xSemaphoreTake(ctx->mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
            if (FAN::DEBUG) printf("[FAN] Mutex busy. Skipping this cycle. \n");
            continue;
        }
        
        // STL's deque
        int new_speed = transition.front();
        transition.pop_front();

        if (FAN::DEBUG) printf("[FAN] New speed: %d. \n", speed.data);
        fan.write(speed.data);
        
        xSemaphoreGive(ctx->mutex);
    }
}
