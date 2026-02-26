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
#include "driver/rom"

#include <stdio.h>
#include <memory>

int main(void)
{
    timer_hw->dbgpause = 0;
    stdio_init_all();

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
    return 0;
}

