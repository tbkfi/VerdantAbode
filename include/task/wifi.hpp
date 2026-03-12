#pragma once

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/apps/http_client.h"
#include <stdio.h>
#include "system.hpp"

#define SSID "***REMOVED***"
#define PASS "***REMOVED***"


namespace Netti {
    constexpr UBaseType_t TASK_PRIORITY = tskIDLE_PRIORITY + 3;
    constexpr uint16_t STACK_DEPTH = 1024;

    class Wifi {
    private:
        const char *ssid;
        const char *pass;
        bool wifi_connection = false;

        static Wifi* instance; 

    public:
        Wifi(const char *ssid0, const char *pass0);
        
        bool connect_wifi();
        bool is_connected();

        static Wifi* get_instance() { return instance; }
        httpc_connection_t settings;
    };

    void task_wifi(void *param);
    void task_create_wifi();
    err_t headers(httpc_state_t *connection, void *arg, struct pbuf *hdr, u16_t hdr_len, u32_t content_len);
    err_t body(void *arg, struct altcp_pcb *conn, struct pbuf *p, err_t err);
    void result(void *arg, httpc_result_t httpc_result, u32_t rx_content_len, u32_t srv_res, err_t err);
}

