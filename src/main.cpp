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
#include "oled.hpp"
#include "mio.hpp"


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
    system.mio_queue = nullptr;

	system.uart = std::make_shared<PicoUart>(SYSTEM::UART_NR, SYSTEM::UART_TX_PIN, SYSTEM::UART_RX_PIN, SYSTEM::BAUD_RATE, SYSTEM::STOP_BITS);
	system.rtu_client = std::make_shared<ModbusClient>(system.uart);

	system.i2c_bus = std::make_shared<PicoI2CBus>(OLED::BUS_NR, 15, 14);
	system.i2c_dev = std::make_shared<PicoI2CDevice>(system.i2c_bus, OLED::ADDR); 
	system.display = std::make_shared<ssd1306>(system.i2c_dev);

	// TEST SCREEN
	system.display->fill(1);
	system.display->show();


	if (system.events == NULL || system.mutex_i2c == NULL || system.mutex_uart == NULL) {
		while (true) printf("[SYSTEM] Initialisation lead to NULL!\n");
	}
	
	// TASKS
	task_create_blink();
	task_create_parser(&system);
	task_create_controller(&system);
	system.input_queue = create_local_inputs();
	system.sdp610_queue = task_create_sdp610(system.mutex_i2c);
	system.gmp252_queue = task_create_gmp252(system.mutex_uart, system.rtu_client);
    system.mio_queue = task_create_mio(system.mutex_uart, system.rtu_client);

	vTaskStartScheduler();
    while(true);
}
