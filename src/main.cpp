/* VerdantAbode
 * main.cpp
 *
 * Tuomo Björk
*/
#include "hardware/timer.h"
extern "C" {
	uint32_t read_runtime_ctr(void) {
		return timer_hw->timerawl;
	}
}

#include "system.hpp"
#include "util.hpp"

#include "blinky.hpp"
#include "sdp610.hpp"
#include "gmp252.hpp"
#include "local_inputs.hpp"
#include "parser.hpp"



#define UART_NR 1
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define BAUD_RATE 9600
#define STOP_BITS 2




int main() {
	// INIT HW
	stdio_init_all();
	init_i2c0();
	init_i2c1();

	// SYSTEM
	SYSTEM::DATA system;
	system.co2_target = 1200;
	system.events = xEventGroupCreate();
	system.mutex_i2c = xSemaphoreCreateMutex();
	system.mutex_uart = xSemaphoreCreateMutex();

    auto uart{std::make_shared<PicoUart>(UART_NR, UART_TX_PIN, UART_RX_PIN, BAUD_RATE, STOP_BITS)};
    auto rtu_client{std::make_shared<ModbusClient>(uart)};

	if (system.events == NULL || system.mutex_i2c == NULL || system.mutex_uart == NULL) {
		while (true) printf("[SYSTEM] Initialisation lead to NULL!\n");
	}
	
	// TASKS
	task_create_blink();
	task_create_parser(&system);
	system.input_queue = create_local_inputs();
	system.sdp610_queue = task_create_sdp610(system.mutex_i2c);
	system.gmp252_queue = task_create_gmp252(system.mutex_uart, rtu_client);

	vTaskStartScheduler();
    while(true);
}
