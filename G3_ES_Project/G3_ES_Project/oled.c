/*
 * oled.c
 *
 * Created: 27.09.2021 12:44:39
 *  Author: amnaw
 */ 

#define OLED_COMMAND 0x1000
#define OLED_DATA 0x1200

#include "oled.h"
#include "fonts.h"

volatile char *oled_command_reg = (char *) OLED_COMMAND;
volatile char *oled_data_reg = (char *) OLED_DATA;

void OLED_init(){
	OLED_write_command(0xae);        //display off
	OLED_write_command(0xa1);        //segment remap
	OLED_write_command(0xda);        //common pads hardware: alternative
	OLED_write_command(0x12);
	OLED_write_command(0xc8);        //common  output scan direction:com63~com0
	OLED_write_command(0xa8);        //multiplex ration mode:63
	OLED_write_command(0x3f);
	OLED_write_command(0xd5);        //display  divide ratio/osc. freq. mode
	OLED_write_command(0x80);
	OLED_write_command(0x81);        //contrast control
	OLED_write_command(0x50);
	OLED_write_command(0xd9);        //set pre-charge period
	OLED_write_command(0x21);
	OLED_write_command(0x20);        //Set Memory Addressing Mode
	OLED_write_command(0x02);			//page addressing mode
	OLED_write_command(0xdb);        //VCOM deselect level mode
	OLED_write_command(0x30);
	OLED_write_command(0xad);        //master configuration
	OLED_write_command(0x00);
	OLED_write_command(0xa4);        //out follows RAM content
	OLED_write_command(0xa6);        //set normal display
	OLED_write_command(0xaf);        // display on
	
	
	//turn everything on
	for (int j = 0; j<8; j++){
		OLED_write_command(0xb0 + j);
		for (int i = 0; i <128; i++){
			OLED_write_data(0b11111111);
		}
	}
	_delay_ms(500);
	OLED_clear();
}

void OLED_clear(){
	for (int j = 0; j<8; j++){
		OLED_write_command(0xb0 + j);
		for (int i = 0; i <128; i++){
			OLED_write_data(0b00000000);
		}
	}
}

void OLED_clear_position(uint8_t row, uint8_t column){
	OLED_pos(row, column);
	OLED_print(' ');
}

void OLED_write_command(char c){
	oled_command_reg[0] = c;
}

void OLED_write_data(char c){
	oled_data_reg[0] = c;
}

void OLED_print(char c){
	int pos = c - 32;
	for (int i = 0; i < 8; i++){
		OLED_write_data(pgm_read_byte(&(font8[pos][i])));
	}
}

void OLED_pos(uint8_t row, uint8_t column){
	OLED_goto_line(row);
	OLED_goto_column(column);
	//for now, when we want to write something, we will be writing from the leftmost positions so as to not deal with the columns at the beginning	
}

void OLED_goto_line(uint8_t line){
	OLED_write_command(0xb0 + (line));		
}

void OLED_goto_column(uint8_t column){
	//OLED_write_command(0x00);
	//OLED_write_command(0x10);
	OLED_write_command(column);
	OLED_write_command(0x10);
}

void OLED_print_string(char* str){
	int i = 0;
	while(str[i] != '\0'){
		OLED_print(str[i]);
		i++;
	}
}

void display_menu(){
	OLED_pos(0,0);
	OLED_print('>');
	OLED_pos(0, 15);
	OLED_print_string("Start Game");
	OLED_pos(2, 15);
	OLED_print_string("Change Brightness");
	OLED_pos(4, 15);
	OLED_print_string("Apple");
	OLED_pos(6, 15);
	OLED_print_string("Banana");
}
