/* VerdantAbode
 * wifi.cpp
 *
 * Tuomo Björk
*/
#include "wifi.hpp"

#include "pico/cyw43_arch.h"
#include "lwip/sockets.h"
#include "lwip/apps/http_client.h"


void WIFI::create_task(SYSTEM::DATA* ctx) {
	xTaskCreate(WIFI::task, "WIFI", WIFI::STACK_DEPTH, (void *)ctx, WIFI::TASK_PRIORITY, NULL);
}

void WIFI::task(void *params) {
	SYSTEM::DATA* ctx = (SYSTEM::DATA*) params;
	int rc = 0;

	// Initialise HW
	if ( (rc = cyw43_arch_init()) ) {
		if (WIFI::DEBUG) printf("[WIFI] (init): FAIL (%d).\n", rc);
		vTaskDelete(NULL);
		return;
	}
	if (WIFI::DEBUG) printf("[WIFI] (init): OK.\n");

	// Client-mode
	cyw43_arch_enable_sta_mode();

	while(true) {
		rc = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);

		if (rc == CYW43_LINK_UP) {
		// Set SYSTEM Link Flag
			xEventGroupSetBits(ctx->events, SYSTEM::FLAG_WIFI_CONNECTED);
			if (WIFI::DEBUG) printf("[WIFI] (link): UP.\n", rc);
			vTaskDelay(WIFI::TASK_INTERVAL); 
		} else {
		// Clear SYSTEM Link Flag
			xEventGroupClearBits(ctx->events, SYSTEM::FLAG_WIFI_CONNECTED);
			if (WIFI::DEBUG) printf("[WIFI] (link): DOWN (%d).\n", rc);

			rc = cyw43_arch_wifi_connect_timeout_ms(WIFI::SSID, WIFI::PASS,
										CYW43_AUTH_WPA2_MIXED_PSK, WIFI::CONN_TIMEOUT_MS);
			
			if (rc == 0) {
				if (WIFI::DEBUG) printf("[WIFI] (connect): OK (IP: %s)\n", ip4addr_ntoa(netif_ip4_addr(netif_default))); 
				vTaskDelay(WIFI::TASK_INTERVAL); 
			} else {
			// Attempt to reconnect a little faster
				if (WIFI::DEBUG) printf("[WIFI] (connect): FAIL (%d)\n", rc);
				vTaskDelay(WIFI::TASK_INTERVAL/4); 
			}
		}
	}
}
