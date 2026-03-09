#include "mqtt/IPStack.h"
#include "mqtt/Countdown.h"


// Wifi
constexpr const char* WIFI_SSID = "SmartIotMQTT";
constexpr const char* WIFI_PASS = "SmartIoT";

class Wifi{
		private:
			const char *ssid;
			const char *pass;
			
			static Wifi* instance;

			bool wifi_connection = false;
			
			Countdown countdown;
			IPStack ipstack;
		public:
			// Constructor
			Wifi(const char *ssid0, const char *pass0);

			// Member functions
			bool connect_wifi();
			bool is_connected();

	};
