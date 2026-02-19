#include "pico/stdlib.h"
#include "driver/i2c.hpp"

#include <stdio.h>

int main(void)
{
	timer_hw->dbgpause = 0;
	stdio_init_all();

	I2CController i2c_1(1, 14, 15); 
	uint8_t payload[10] = "ALPHA\n";
	uint8_t recovered[10] = "" ;

	//i2c_1.write(0x50, (const uint8_t*)payload, sizeof(payload), 0);
	i2c_1.read(0x50, recovered, sizeof(payload), 0);
	printf(">> %s\n", (uint8_t*)recovered);
	return 0;
}
