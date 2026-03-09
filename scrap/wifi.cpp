#include "wifi.hpp"
#include "system.hpp"
#include <stdio.h>

namespace Netti {

    Wifi* Wifi::instance = nullptr;

    Wifi::Wifi(const char *ssid0, const char *pass0)
        : ssid(ssid0), pass(pass0), ipstack() 
    {
        instance = this;
    }

    bool Wifi::connect_wifi() {
        int return_code = ipstack.connect(IP_ADDR, PORT); 
        
        this->wifi_connection = (return_code == 0);
        return this->wifi_connection;
    }

    bool Wifi::is_connected() { 
        return wifi_connection; 
    }

    void task_create_wifi() {
        xTaskCreate(task_wifi, "Wifi_Task", STACK_DEPTH, NULL, TASK_PRIORITY, NULL);
    }

    void task_wifi(void *param) {
        while (true) {
            Wifi* net = Wifi::get_instance();

            if (net == nullptr) {
                vTaskDelay(pdMS_TO_TICKS(100));
                continue;
            }

            for (int i = 0; i < 3; i++) {
                printf("[Wi-Fi] Attempt %d/3...\n", i + 1);
                
                if (net->connect_wifi()) {
                    printf("Connected!\n");
                    xEventGroupSetBits(events, SYSTEM::DATA::FLAG_WIFI_CONNECTED);
                    break; 
                }

                printf("Attempt %d/3 failed.\n", i + 1);
                
                vTaskDelay(pdMS_TO_TICKS(2000));
               // TO DO: 
               // WHAT HAPPENS WHEN CONNECTION FAILS 3 TIMES? 
            }
   
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }
}
