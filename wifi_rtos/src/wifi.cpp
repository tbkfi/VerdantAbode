#include "wifi.hpp"
#include <stdio.h>
#include <string.h>
#include "pico/cyw43_arch.h"
#include "FreeRTOS.h"
#include "task.h"

// Buffer for incoming HTTP responses
char myBuff[1024];
// Flag to prevent overlapping async requests
static volatile bool is_transferring = false;

namespace Netti {

    Wifi* Wifi::instance = nullptr;

    Wifi::Wifi(const char *ssid0, const char *pass0)
        : ssid(ssid0), pass(pass0), wifi_connection(false) 
    {
        instance = this;
        // Bind the callback functions to the settings struct
        this->settings.result_fn = result;
        this->settings.headers_done_fn = headers;
    }

    bool Wifi::connect_wifi() {
        static bool hardware_initialized = false;
        
        if (!hardware_initialized) {
            printf("[Hardware]: Initialising CYW43...\n");
            if (cyw43_arch_init_with_country(CYW43_COUNTRY_UK)) {
                printf("[Hardware]: Failed to initialise chip\n");
                return false;
            }
            cyw43_arch_enable_sta_mode();
            hardware_initialized = true;
        }

        if (cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA) == 3) {
            return true;
        }

        printf("[Wi-Fi]: Connecting to %s...\n", ssid);
        int rc = cyw43_arch_wifi_connect_timeout_ms(ssid, pass, CYW43_AUTH_WPA2_AES_PSK, 15000);
        
        if (rc != 0) {
            printf("[Wi-Fi]: Connect failed! (rc: %d)\n", rc);
            wifi_connection = false;
            return false;
        }

        printf("[Wi-Fi]: Connected successfully!\n");
        wifi_connection = true;
        return true;
    }

    bool Wifi::is_connected() {
        return cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA) == 3;
    }

    // --- lwIP HTTP Callbacks ---

    err_t headers(httpc_state_t *connection, void *arg, 
                  struct pbuf *hdr, u16_t hdr_len, u32_t content_len)
    {
        printf("[HTTP]: Response headers received. Content length: %u\n", content_len);
        return ERR_OK;
    }

    err_t body(void *arg, struct altcp_pcb *conn, struct pbuf *p, err_t err)
    {
        // Copy data and ensure we don't overflow myBuff
        size_t len = (p->tot_len > 1023) ? 1023 : p->tot_len;
        pbuf_copy_partial(p, myBuff, len, 0);
        myBuff[len] = '\0'; // Crucial: Null terminate so printf works
        
        printf("[HTTP]: Body content: %s\n", myBuff);
        return ERR_OK;
    }

    void result(void *arg, httpc_result_t httpc_result,
                u32_t rx_content_len, u32_t srv_res, err_t err)
    {
        // 1 = Success in ThingSpeak usually means it wrote the entry
        printf("[HTTP]: Transfer finished. HTTP Status: %u, Result: %d\n", srv_res, httpc_result);
        is_transferring = false; 
    }

    // --- Task Implementation ---

    void task_wifi(void *param) {
        printf("[Task]: Wifi Task Started\n");
        
        while (true) {
            Wifi* net = Wifi::get_instance();
            if (net == nullptr) {
                vTaskDelay(pdMS_TO_TICKS(1000));
                continue;
            }

            // Ensure we have a connection
            if (!net->is_connected()) {
                if (!net->connect_wifi()) {
                    vTaskDelay(pdMS_TO_TICKS(5000));
                    continue;
                }
            }

            // ONLY send if a previous transfer isn't already running
            if (!is_transferring) {
                is_transferring = true;

                // HARDCODED VALUES FOR TESTING
                char url_query[256];
                snprintf(url_query, sizeof(url_query), 
                         "/update?api_key=***REMOVED***&field1=2.11&field2=26&field3=3.39&field4=1");

                printf("[HTTP]: Requesting: %s\n", url_query);

                err_t err = httpc_get_file_dns(
                    "api.thingspeak.com", 
                    80, 
                    url_query, 
                    &net->settings, 
                    body, 
                    NULL, 
                    NULL
                );

                if (err != ERR_OK) {
                    printf("[HTTP]: Error starting request: %d\n", err);
                    is_transferring = false;
                }
            }

            // ThingSpeak Free Tier requires 15 seconds between updates.
            // We wait 30 seconds to be safe.
            for (int i = 0; i < 300; i++) {
                cyw43_arch_poll(); 
                vTaskDelay(pdMS_TO_TICKS(100)); 
            }
        }
    }

    void task_create_wifi() {
        xTaskCreate(task_wifi, "Wifi_Task", 4096, NULL, 1, NULL);
    }
}
