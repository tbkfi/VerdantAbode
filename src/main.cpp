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
#include "eeprom.hpp"

#include "blinky.hpp"
#include "parser.hpp"
#include "controller.hpp"
#include "local_inputs.hpp"
#include "oled.hpp"
#include "sdp610.hpp"
#include "gmp252.hpp"
#include "hmp60.hpp"
#include "mio.hpp"
#include "valve.hpp"


int main() {
	timer_hw->dbgpause = 0;

	// HARDWARE initialisation
	stdio_init_all();
	printf("<BOOT>\n");

	init_i2c0();
	init_i2c1();

	// SYSTEM initialisation
	static SYSTEM::DATA system;
	EEPROM::load(&system, system.mutex_i2c);

	system.uart = std::make_shared<PicoUart>(SYSTEM::UART_NR, SYSTEM::UART_TX_PIN, SYSTEM::UART_RX_PIN, SYSTEM::BAUD_RATE, SYSTEM::STOP_BITS);
	system.rtu_client = std::make_shared<ModbusClient>(system.uart);

	system.i2c_bus = std::make_shared<PicoI2CBus>(OLED::BUS_NR, 15, 14);
	system.i2c_dev = std::make_shared<PicoI2CDevice>(system.i2c_bus, OLED::ADDR); 

	system.display = std::make_shared<ssd1306>(system.i2c_dev);
	
	// TASK creation
	BLINKY::create_task();
	PARSER::create_task(&system);
	CONTROLLER::create_task(&system);
	OLED::create_task(&system);
	VALVE::create_task(&system);
	system.input_queue = LOCAL_INPUTS::create();
	system.sdp610_queue = SDP610::create_task(system.mutex_i2c);
	system.gmp252_queue = GMP252::create_task(system.mutex_uart, system.rtu_client);
	system.hmp60_queue = HMP60::create_task(system.mutex_uart, system.rtu_client);
	system.mio_queue = FAN::create_task(system.mutex_uart, system.rtu_client);

	vTaskStartScheduler();
    while(true);
}
