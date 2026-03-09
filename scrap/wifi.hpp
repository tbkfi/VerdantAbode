#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "mqtt/IPStack.h"
#include "mqtt/Countdown.h"
#include "mqtt/MQTTClient.h" // Assuming you use the Paho MQTT client

namespace Netti {
    constexpr bool DEBUG = true;
    constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 3;
    constexpr uint16_t STACK_DEPTH = 512;
    constexpr uint8_t QUE_LEN = 8;
    constexpr uint8_t INTEGRATION_TIME_MS = 8;

    struct CTX {
        QueueHandle_t que;
        SemaphoreHandle_t mutex;
    };

    struct QUE_ELEMENT {
        uint32_t time_ms;
        int16_t data;
    };

    class Wifi {
    private:
        const char *ssid;
        const char *pass;
        bool wifi_connection = false;
        IPStack ipstack;
        MQTT::Client<IPStack, Countdown> client; // Added missing client type

        static Wifi* instance;

    public:
        Wifi(const char *ssid0, const char *pass0);
        bool connect_wifi();
        bool is_connected();
        static Wifi* get_instance() { return instance; }
        
        // Placeholders for your specific network config
        const char* broker_ip = "192.168.1.10"; 
        int broker_port = 1883;
    };

    void task_wifi(void *param);
    void task_create_wifi(SemaphoreHandle_t mutex_i2c);
}

