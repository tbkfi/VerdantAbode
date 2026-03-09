#include "wifi.hpp"
#include <stdio.h>

namespace Netti {

    // Initialize static member
    Wifi* Wifi::instance = nullptr;

    Wifi::Wifi(const char *ssid0, const char *pass0)
        : ssid(ssid0), pass(pass0), ipstack(), client(ipstack) 
    {
        instance = this;
        this->connect_wifi();
    }

    bool Wifi::connect_wifi() {
        // Note: Ensure ipstack.connect is implemented for your specific Wi-Fi chip (CYW43)
        int return_code = ipstack.connect(this->broker_ip, this->broker_port);
        if (return_code == 0) {
            this->wifi_connection = true;
            return true;
        }
        this->wifi_connection = false;
        return false;
    }

    bool Wifi::is_connected() { 
        return wifi_connection; 
    }

    void task_create_wifi(SemaphoreHandle_t mutex_i2c) {
        static SemaphoreHandle_t mutex;
        mutex = mutex_i2c;
        xTaskCreate(WiFi::task_wifi, "Wi-Fi_Task", Netti::STACK_DEPTH, (void *)&mutex, Netti::TASK_PRIORITY, NULL);
    }

    void task_wifi(void* param) {
        Wifi* net = Wifi::get_instance();

        while (true) {
            if (net == nullptr) {
                vTaskDelay(pdMS_TO_TICKS(100));
                net = Wifi::get_instance();
                continue;
            }

            if (Netti::DEBUG) printf("[Wi-Fi] Checking connection...\n");

            // Use the semaphore to protect I2C or Shared SPI bus if Wi-Fi and I2C share lines
            if (xSemaphoreTake(mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
                
                if (!net->is_connected()) {
                    if (!net->connect_wifi()) {
                        if (Netti::DEBUG) printf("[Wi-Fi] Connection failed!\n");
                    } else {
                        if (Netti::DEBUG) printf("[Wi-Fi] Connection successful!\n");
                    }
                }

                xSemaphoreGive(mutex);
            }

            vTaskDelay(pdMS_TO_TICKS(2000)); // Don't spam the connection logic
        }
    }
} 
