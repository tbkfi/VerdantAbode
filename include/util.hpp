#pragma once

#include <stdio.h>
#include <stdint.h>
#include "FreeRTOS.h"

uint32_t uptime_ms(void);
void init_i2c0(void);
void init_i2c1(void);
