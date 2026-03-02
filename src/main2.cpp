#include <stdio.h>

#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "hardware/timer.h"
extern "C" {
	uint32_t read_runtime_ctr(void) {
		return timer_hw->timerawl;
	}
}

struct led_params{
    uint pin;
    uint delay;
};



// Assorted Utils
uint64_t current_time_ms(void) {
	return time_us_64() / 1000 ;
}



void blink_task(void *param)
{
    led_params *lpr = (led_params *) param;
    const uint led_pin = lpr->pin;
    const uint delay = lpr->delay;

    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    while (true) {
        gpio_put(led_pin, 1); vTaskDelay(delay);
        gpio_put(led_pin, 0); vTaskDelay(delay);
		printf("[%llu] blink\n", current_time_ms());
    }
}


struct system {
	uint32_t co2_setpoint;
	QueueHandle_t co2;
	QueueHandle_t pressure;
	QueueHandle_t mio;
};

void task_sdp600(void *param) {
// SDP600 (pressure sensor) from Sensirion via I2C
}

void task_gmp252(void *param) {
// GMP252 (CO2 probe) from Vaisala via Modbus
}

void task_hmp60(void *param) {
// HMP60 (Rh and T sensor) from Vaisala via Modbus
}

void task_mio(void *param) {
// MIO-12 (IO-device) from Produal via Modbus
}


int main() {
	stdio_init_all();

	// Blinky (sanity check)
    static led_params lp1 = { .pin = 20, .delay = 250 };
    static led_params lp2 = { .pin = 21, .delay = 450 };
    xTaskCreate(blink_task, "LED_1", 256, (void *) &lp1, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(blink_task, "LED_2", 256, (void *) &lp2, tskIDLE_PRIORITY + 1, NULL);

	// SDP60
	
	// GMP252
	
	// HMP60
	
	// MIO


    vTaskStartScheduler();
    while(true);
}
