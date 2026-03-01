#include <stdio.h>

#include "MQTTClient.h"
#include "MQTTConnect.h"
#include "MQTTPacket.h"
#include "mqtt/IPStack.h"
#include "mqtt/Countdown.h"


// Wifi & MQTT
constexpr const char* WIFI_SSID = "SmartIotMQTT";
constexpr const char* WIFI_PASS = "SmartIoT";

constexpr const char* MQTT_BROKER_IP = "192.168.50.140";
constexpr int MQTT_BROKER_PORT = 1883;

class WifiMQTT{
	// As the name implies, this class could do
	// with splitting into two, but time constraints
	// are secondary to functionality for now.
		private:
			const char *ssid;
			const char *pass;
			const char *broker_ip;
			const int broker_port;
			
			static WifiMQTT* instance;

			bool wifi_connection = false;
			bool mqtt_connection = false;
			
			char last_mqtt_msg[128];
			int last_mqtt_msg_len = 0;
			bool msg_handled = false;
			
			Countdown countdown;
			IPStack ipstack;
			MQTT::Client<IPStack, Countdown> client;
			MQTTPacket_connectData data;
		public:
			// Constructor
			WifiMQTT(const char *ssid0, const char *pass0,
					const char *broker_ip0, int broker_port0);

			// Member functions
			bool connect_wifi();
			bool connect_mqtt();
			bool is_connected();

			bool mqtt_subscribe(const char* topic);
			bool mqtt_send_msg(const char* msg);
			bool new_msg();
			
         //Config::GDOOR_STATE mqtt_get_cmd(); REPLACE LATER IF NEEDED

			void loop(int timeout_ms);

			void messageArrived(MQTT::MessageData &md);
			static void messageArrivedWrapper(MQTT::MessageData &md);
	};
