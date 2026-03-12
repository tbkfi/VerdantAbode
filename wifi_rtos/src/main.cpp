#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "wifi.hpp"
// sleep_ms equivalent for FreeRTOS: vTaskDelay((100));
uint32_t read_runtime_ctr(void) {
    return timer_hw->timerawl;
}


int main(){

   stdio_init_all();
   sleep_ms(2000);
   printf("\nBoot\n");
   static Netti::Wifi my_wifi(SSID, PASS);
   Netti::task_create_wifi();
   vTaskStartScheduler(); 
   while (true) cyw43_arch_poll();
}



