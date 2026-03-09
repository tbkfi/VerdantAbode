#include "oled_config.hpp"

auto bus = std::make_shared<PicoI2CBus>(I2C_UNIT, 15, 14);
auto dev = std::make_shared<PicoI2CDevice>(bus, ADDR); 
ssd1306 display(dev);

void ssd1306_screen(){
    display.fill(0); // clear screen
    display.text("[Co2]: ",      0, 0, 1);
    display.text("[Temp]: ",     0, 13, 1);
    display.text("[P]: ",        0, 26, 1);
    display.text("[WiFi]: ",     0, 39, 1);
    display.text("[A <- -> B]",  0, 52, 1);
}

void ssd1306_c02(int16_t co2){
   char buffer[16]; // A small array to hold the converted number
    // snprintf converts the integer 'co2' into a string inside 'buffer'
   snprintf(buffer, sizeof(buffer), "%d", co2);
   display.text(buffer, 50, 0, 1);
   display.text("ppm", 70, 0, 1);
   display.show();
}

void ssd1306_temp(uint32_t temp){
    // snprintf converts the integer 'co2' into a string inside 'buffer'
   snprintf(buffer, sizeof(buffer), "%d", temp);
   display.text(buffer, 50, 13, 1);
   display.text("C", 70, 13, 1);
   display.show();
}

void ssd1306_pressure(uint32_t press){
   char buffer[16]; // A small array to hold the converted number
    // snprintf converts the integer 'co2' into a string inside 'buffer'
   snprintf(buffer, sizeof(buffer), "%d", press);
   display.text(buffer, 50, 26, 1);
   display.text("Pa", 70, 26, 1);
   display.show();
}

void ssd1306_wifi(bool status){
   char buffer[16]; // A small array to hold the converted number
// snprintf converts the integer 'co2' into a string inside 'buffer'
   snprintf(buffer, sizeof(buffer), "%d", status);
   display.text(buffer, 50, 39, 1);
   display.show();
}

void ssd1306_draw_fullscreen(int16_t co2, uint32_t temp, uint32_t press, bool status){
  ssd1306_screen();
  ssd1306_c02(co2);
  ssd1306_temp(temp);
  ssd1306_temp(press);
  ssd1306_wifi(status);
  display.draw();
}
