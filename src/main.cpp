// #include "FreeRTOS.h"
// #include "task.h"
// #include "pico/stdlib.h"
// 
// #include "hardware/timer.h"
// extern "C" {
// uint32_t read_runtime_ctr(void) {
//     return timer_hw->timerawl;
// }
// }
// 
// struct led_params{
//     uint pin;
//     uint delay;
// };
// 
// void blink_task(void *param)
// {
//     led_params *lpr = (led_params *) param;
//     const uint led_pin = lpr->pin;
//     const uint delay = lpr->delay;
// 
//     gpio_init(led_pin);
//     gpio_set_dir(led_pin, GPIO_OUT);
// 
//     while (true) {
//         gpio_put(led_pin, 1); vTaskDelay(delay);
//         gpio_put(led_pin, 0); vTaskDelay(delay);
//     }
// }
// 
// int main() {
//     static led_params lp1 = { .pin = 20, .delay = 250 };
//     static led_params lp2 = { .pin = 21, .delay = 500 };
// 
//     xTaskCreate(blink_task, "LED_1", 256, (void *) &lp1, tskIDLE_PRIORITY + 1, NULL);
//     xTaskCreate(blink_task, "LED_2", 256, (void *) &lp2, tskIDLE_PRIORITY + 1, NULL);
// 
//     vTaskStartScheduler();
// 
//     while(true);
// }
//Example of using I2CController class

#include "pico/stdlib.h"
#include "driver/rom.hpp"
#include "driver/i2c.hpp"
#include "modbus/register.hpp"
#include "modbus/client.hpp"

#include <driver/uart.hpp>

#include <stdio.h>
#include <memory>


#define UART_NR 1
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define BAUD_RATE 9600
#define STOP_BITS 1

int main(void)
{
    timer_hw->dbgpause = 0;
    stdio_init_all();

    auto uart{std::make_shared<PicoUart>(UART_NR, UART_TX_PIN, UART_RX_PIN, BAUD_RATE, STOP_BITS)};
    auto rtu_client{std::make_shared<ModbusClient>(uart)};

    //ModbusRegister gmp252 (rtu_client, 240, 1);
    ModbusRegister gmp252 (rtu_client, 2049, 0x0800);

    while (true) {
        auto modbus_poll = make_timeout_time_ms(3000);
        printf(">> %u \n", gmp252.read());
        sleep_ms(100);
        //ModbusRegister produal(rtu_client, 1, 0);
        //produal.write(100);
    }

#if 0
    std::shared_ptr<I2CController> i2c_1 = 
        std::make_shared<I2CController>(1, 14, 15);

    EEPROM rom(i2c_1, 0x50);
    uint8_t payload[10] = "ALPHA\n";
    uint8_t recovered[10] = "" ;

    rom.write((const uint8_t*)payload, sizeof(payload), 10);
    rom.read(recovered, sizeof(payload), 10);

    //i2c_1.write(0x50, (const uint8_t*)payload, sizeof(payload), 0);
    //i2c_1.read(0x50, recovered, sizeof(payload), 0);
    printf(">> %s\n", (uint8_t*)recovered);
#endif
    return 0;
}


