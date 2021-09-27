/*
 * oled.h
 *
 * Created: 27.09.2021 12:44:54
 *  Author: amnaw
 */ 


#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

void OLED_init();
void OLED_write_data(char c);

void OLED_reset();
void OLED_home();
void OLED_goto_line(uint8_t line);
void OLED_clear_line(uint8_t line);
void OLED_pos(uint8_t row, uint8_t column);
void OLED_print(char* c);
void OLED_set_brightness(uint8_t lvl);