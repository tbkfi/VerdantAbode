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
#include "parser.hpp"
#include "controller.hpp"
#include "local_inputs.hpp"
#include "sdp610.hpp"
#include "gmp252.hpp"
#include "wifi.hpp"
#include "oled_config.hpp"

int main() {
	timer_hw->dbgpause = 0;

	// HARDWARE initialisation
	stdio_init_all();
	printf("<BOOT>\n");

	init_i2c0();
	init_i2c1();

	// SYSTEM initialisation
	static SYSTEM::DATA system;
	system.co2_target = SYSTEM::CO2_TARGET;
	system.events = xEventGroupCreate();
	system.mutex_i2c = xSemaphoreCreateMutex();
	system.mutex_uart = xSemaphoreCreateMutex();
	system.input_queue = nullptr;
	system.sdp610_queue = nullptr;
	system.gmp252_queue = nullptr;

	system.uart = std::make_shared<PicoUart>(SYSTEM::UART_NR, SYSTEM::UART_TX_PIN, SYSTEM::UART_RX_PIN, SYSTEM::BAUD_RATE, SYSTEM::STOP_BITS);
	system.rtu_client = std::make_shared<ModbusClient>(system.uart);


	if (system.events == NULL || system.mutex_i2c == NULL || system.mutex_uart == NULL) {
		while (true) printf("[SYSTEM] Initialisation lead to NULL!\n");
	}
	
	// TASKS
	task_create_blink();
	task_create_parser(&system);
	task_create_controller(&system);
   task_create_ssd1306(system.mutex_i2c);
	system.input_queue = create_local_inputs();
	system.sdp610_queue = task_create_sdp610(system.mutex_i2c);
	system.gmp252_queue = task_create_gmp252(system.mutex_uart, system.rtu_client);

	vTaskStartScheduler();
    while(true);
}
