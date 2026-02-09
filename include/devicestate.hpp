#pragma once

#include <cstdint>

#include "namespace/config.hpp"

#include "dev/gmp252.hpp"
#include "dev/hmp60.hpp"
#include "dev/sdp610.hpp"

struct sensor_reading_t {
	uint32_t time;
	int16_t reading;
};


class DeviceState {
	private:
		// IO
		QueueHandle_t cmd_local;
		QueueHandle_t cmd_remote;
		// Sensors
		dev_eeprom_t eeprom;
		dev_hmp60_t hmp60;
		dev_gmp252_t gmp252;
		dev_sdp610 sdp610;
		// Devices
		dev_produalmio_t mio;
		dev_ssd1306_t oled;
		dev_rotary_t rot;
		dev_button_t btn1;
		dev_button_t btn2;
		dev_button_t btn3;
		dev_led_t led1;
		dev_led_t led2;
		dev_led_t led3;
		// dev_wifi_t wifi // note: add later
		// Device State
		sensor_reading_t data_gmp252[Config::SENSOR_ARR_LEN];
		sensor_reading_t data_hmp60[Config::SENSOR_ARR_LEN];
		sensor_reading_t data_sdp610[Config::SENSOR_ARR_LEN];
	public:
		DeviceState () : {}
};
