#include "pico/stdlib.h"
#include "ssd1306.h"
#include "PicoI2CDevice.h"

void print_to_display(char* ctx);
void draw_screen();
void refresh_screen();
void draw_c02(int co2);
void draw_temp(int temp);
void draw_pressure(int press); 
void draw_wifi(int status); 

