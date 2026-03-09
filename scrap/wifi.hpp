#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "mqtt/IPStack.h"

#define PORT 1883
#define IP_ADDR "168.128.0.1"

namespace Netti {
    constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 3;
    constexpr uint16_t STACK_DEPTH = 1024;

    class Wifi {
    private:
        const char *ssid;
        const char *pass;
        bool wifi_connection = false;
        IPStack ipstack;

        static Wifi* instance; 
    public:
        Wifi(const char *ssid0, const char *pass0);
        
        bool connect_wifi();
        bool is_connected();

        static Wifi* get_instance() { return instance; }
    };

    void task_wifi(void *param);
    void task_create_wifi();
}

