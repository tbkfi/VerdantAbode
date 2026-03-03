#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

struct led_params{
    uint pin;
    uint delay;
};

void blink_task(void *param);
void task_create_blink(void);
