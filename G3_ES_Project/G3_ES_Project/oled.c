/*
 * oled.c
 *
 * Created: 27.09.2021 12:44:39
 *  Author: amnaw
 */ 

#define OLED_BASE 0x1000

#include "oled.h"
#include "fonts.h"

volatile char *oled_reg = (char *) OLED_BASE;

void OLED_init(){
	OLED_write_data(0xae);        //display off
	OLED_write_data(0xa1);        //segment remap
	OLED_write_data(0xda);        //common pads hardware: alternative
	OLED_write_data(0x12);
	OLED_write_data(0xc8);        //common  output scan direction:com63~com0
	OLED_write_data(0xa8);        //multiplex ration mode:63
	OLED_write_data(0x3f);
	OLED_write_data(0xd5);        //display  divide ratio/osc. freq. mode
	OLED_write_data(0x80);
	OLED_write_data(0x81);        //contrast control
	OLED_write_data(0x50);
	OLED_write_data(0xd9);        //set pre-charge period
	OLED_write_data(0x21);
	OLED_write_data(0x20);        //Set Memory Addressing Mode
	OLED_write_data(0x02);			//page addressing mode
	OLED_write_data(0xdb);        //VCOM deselect level mode
	OLED_write_data(0x30);
	OLED_write_data(0xad);        //master configuration
	OLED_write_data(0x00);
	OLED_write_data(0xa4);        //out follows RAM content
	OLED_write_data(0xa6);        //set normal display
	OLED_write_data(0xaf);        // display on
}

void OLED_write_data(char c){
	
}

void OLED_pos(uint8_t row, uint8_t column){
	OLED_goto_line(row);
	OLED_write_data(0x00);
	//for now, when we want to write something, we will be writing from the leftmost positions so as to not deal with the columns at the beginning
}

void OLED_goto_line(uint8_t line){
	OLED_write_data(0xb0 + (line - 1));		//assuming lines are from 1 to 8 and not 0 to 7
}