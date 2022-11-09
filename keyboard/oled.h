#ifndef OLED_H
#define OLED_H
#include <Arduino.h>

void oled_init(uint8_t sda, uint8_t clk);
void oled_sendByte(uint8_t data);
void oled_start_commands();
void oled_start_data();
void oled_disable();
void oled_home();

#endif
