#include "task/wifi_send.hpp"
#include "lwip/tcp.h"


static void callback_httpc_result(void *arg, httpc_result_t httpc_result, u32_t rx_content_len, u32_t srv_res, err_t err) {
// Fires on: HTTP Result
	if (WIFI_SEND::DEBUG) {
		printf("[WIFI_SEND] (http_cb): lwip (%d), http (%d)\n", httpc_result, srv_res);
	}
}

static err_t callback_httpc_headers(httpc_state_t *connection, void *arg, struct pbuf *hdr, u16_t hdr_len, u32_t content_len) {
// Fires on: HTTP Headers
	return ERR_OK;
}

static err_t callback_httpc_body(void *arg, struct tcp_pcb *conn, struct pbuf *p, err_t err) {
// Fires on: HTTP Body
	if (p != NULL) {
		tcp_recved(conn, p->tot_len);
		pbuf_free(p);
	}
	return ERR_OK;
}


static const httpc_connection_t settings = {
// see: https://www.nongnu.org/lwip/2_1_x/group__httpc.html
	.use_proxy = 0,
	.result_fn = callback_httpc_result,
	.headers_done_fn = callback_httpc_headers,
};


void WIFI_SEND::create_task(SYSTEM::DATA* ctx) {
	xTaskCreate(WIFI_SEND::task, "WIFI_SEND", WIFI_SEND::STACK_DEPTH, (void *) ctx, WIFI_SEND::TASK_PRIORITY, NULL);
}

void WIFI_SEND::task(void *param) {
	SYSTEM::DATA* ctx = (SYSTEM::DATA*) param;

	TickType_t last_ran = xTaskGetTickCount();
	const TickType_t poll_time = pdMS_TO_TICKS(WIFI_SEND::TASK_INTERVAL_MS);

	char post[256];
	while (true) {
		vTaskDelayUntil(&last_ran, poll_time);

		EventBits_t flags = xEventGroupGetBits(ctx->events);
		bool wifi_connected = (flags & SYSTEM::FLAG_WIFI_CONNECTED);

		if (!wifi_connected) {
			if (WIFI_SEND::DEBUG) printf("[WIFI_SEND] (post): Link DOWN.\n");
			continue;
		}

		int16_t co2  = ctx->val_co2;
		float   temp = ctx->val_temp;
		float   rh   = ctx->val_rh;
		float   pa   = ctx->val_pa;

		snprintf(post, sizeof(post), 
			"/update?api_key=%s&field1=%d&field2=%.2f&field3=%.2f&field4=%.2f", 
			WIFI_SEND::API_KEY, co2, temp, rh, pa);

		if (WIFI_SEND::DEBUG) printf("[WIFI_SEND] (post): %s\n", post);

		// Begin & End transaction
		cyw43_arch_lwip_begin();
		err_t rc = httpc_get_file_dns(
			"api.thingspeak.com", 
			80, 
			post, 
			&settings, 
			callback_httpc_body, 
			NULL, 
			NULL
		);
		cyw43_arch_lwip_end();

		// Debug
		if (rc == ERR_OK) {
			if (WIFI_SEND::DEBUG) printf("[WIFI_SEND] (post): OK\n");
		} else {
			if (WIFI_SEND::DEBUG) printf("[WIFI_SEND] (post): FAIL (%d)\n", rc);
		}
	}
}
