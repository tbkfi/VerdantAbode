#include "wifi.hpp" 

// WifiMQTT
	WifiMQTT::WifiMQTT(const char *ssid0, const char *pass0, const char *broker_ip0, int broker_port0)
	: ssid(ssid0), pass(pass0),
	broker_ip(broker_ip0), broker_port(broker_port0),
	ipstack(ssid0, pass0), client(ipstack, 1000)  
	{
	this->instance = this;
	this->connect_wifi();
	//this->connect_mqtt();
	}

	void WifiMQTT::loop(int timeout_ms) { client.yield(timeout_ms); }

	bool WifiMQTT::connect_wifi() {
		int return_code = ipstack.connect(this->broker_ip, this->broker_port);
		if (return_code == 0) {
			this->wifi_connection = true;
			return true;
		}
		this->wifi_connection = false;
		return false;
	}

	bool WifiMQTT::connect_mqtt() {
		if (!this->wifi_connection) return false;
		
		MQTTPacket_connectData data = MQTTPacket_connectData_initializer; 
		data.MQTTVersion = 4;
		data.clientID.cstring = (char *) "PicoW-sample";

		int return_code = client.connect(data);
		if (return_code == 0) {
			this->mqtt_connection = true;
			return true;
		}
		this->mqtt_connection = false;
		return false;
	}

	bool WifiMQTT::is_connected() { return wifi_connection && mqtt_connection; }

	bool WifiMQTT::mqtt_subscribe(const char* topic) {
		if (!this->is_connected()) return false;
		
		int return_code = client.subscribe(topic, MQTT::QOS0, messageArrivedWrapper);
		return (return_code == 0);
	}

	bool WifiMQTT::mqtt_send_msg(const char *msg){
		if (!this->is_connected() && !msg) return false;
		
		MQTT::Message respMsg;
		respMsg.qos = MQTT::QOS0;
		respMsg.retained = false;
		respMsg.payload = (void*)msg;
		respMsg.payloadlen = strlen(msg);
		
		int return_code = client.publish("garage/door/status", respMsg);
		return (return_code == 0);
	}

	bool WifiMQTT::new_msg() {
		if (!this->msg_handled) return true;
		else return false;
	}

	Config::GDOOR_STATE WifiMQTT::mqtt_get_cmd(){
		Config::GDOOR_STATE new_state = Config::GDOOR_STATE::BAD_CMD;
		if (!strcmp(last_mqtt_msg, "OPEN")) {
			new_state = Config::GDOOR_STATE::OPENING;
			this->msg_handled = true;
		}
		else if (!strcmp(last_mqtt_msg, "CLOSE")) {
			new_state = Config::GDOOR_STATE::CLOSING;  
			this->msg_handled = true;
		}

		return new_state;
	}

	void WifiMQTT::messageArrived(MQTT::MessageData &md){
		MQTT::Message &msg = md.message;

		// get bounds
		this->last_mqtt_msg_len = (msg.payloadlen < sizeof(last_mqtt_msg) - 1)
			? msg.payloadlen : sizeof(last_mqtt_msg) - 1;
		
		// save new msg
		memcpy(this->last_mqtt_msg, msg.payload, last_mqtt_msg_len);
		this->last_mqtt_msg[last_mqtt_msg_len] = '\0';
		this->msg_handled = false;
	}

	WifiMQTT* WifiMQTT::instance = nullptr; // Trampoline-stuff

	void WifiMQTT::messageArrivedWrapper(MQTT::MessageData &md) {
	// This gets called by MQTT library when RX msg!
		if (instance) instance->messageArrived(md);
	}
};
