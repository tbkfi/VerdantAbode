#include "wifi.hpp" 

// WifiMQTT
	Wifi::Wifi(const char *ssid0, const char *pass0)
	: ssid(ssid0), pass(pass0),
	ipstack(ssid0, pass0), client(ipstack, 1000)  
	{
	this->instance = this;
	this->connect_wifi();
	//this->connect_mqtt();
	}


	bool Wifi::connect_wifi() {
		int return_code = ipstack.connect(this->broker_ip, this->broker_port);
		if (return_code == 0) {
			this->wifi_connection = true;
			return true;
		}
		this->wifi_connection = false;
		return false;
	}


	bool Wifi::is_connected() { return wifi_connection; }



	Wifi* Wifi::instance = nullptr; // Trampoline-stuff


};
