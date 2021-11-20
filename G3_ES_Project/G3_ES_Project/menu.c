/*
 * menu.c
 *
 * Created: 04.10.2021 13:30:20
 *  Author: kathersi
 */ 


#define F_CPU 4915200

#include "joystick.h"
#include "oled.h"
#include "uart.h"
#include "sram.h"
#include "adc.h"
#include "slider.h"
#include "menu.h"
#include "can.h"
#include "spi.h"
#include "mcp2515.h"

#define HORIZONTAL 0
#define VERTICAL 1

static int centerX, centerY;
int currentRow;
int playing = 0;

void menu_init(void){
	centerX = calibrate_joystick_center(0, 10);
	centerY = calibrate_joystick_center(1, 10);
	display_main_menu();
	navigate_menu();
}

void display_main_menu(void){
	OLED_clear();
	OLED_pos(0,0);
	OLED_print('>');
	OLED_pos(0, 15);
	OLED_print_string("Start Game");
	OLED_pos(2, 15);
	OLED_print_string("Brightness");
	OLED_pos(4, 15);
	OLED_print_string("Instructions");
	OLED_pos(6, 15);
	OLED_print_string("Display Center");
	currentRow = 0;
}

void navigate_menu(void){
	if(!playing){
	printf("current row before while: %d\r\n", currentRow);
		while(1){
			uint8_t x = adc_read(0);
			uint8_t y = adc_read(1);
			printf("adc horizontal: %d, adc vertical: %d\n\r", x, y);
			if(normalize_output_joystick(x, centerX) < -90){
				display_main_menu();
				_delay_ms(1000);
			}
		
			if(normalize_output_joystick(x, centerX) > 90){
				printf("current row %d\r\n", currentRow);
				select_item(currentRow);
				_delay_ms(1000);
			}
		
			if(normalize_output_joystick(y, centerY) > 90){
				OLED_clear_position(currentRow, 0);
				currentRow = currentRow - 2;
				if(currentRow < 0){
					currentRow = 6;
				}
				printf("current row after moving up: %d\r\n", currentRow);
				OLED_pos(currentRow, 0);
				OLED_print('>');
				_delay_ms(1000);
			}
		
			if(normalize_output_joystick(y, centerY) < -90){
				OLED_clear_position(currentRow, 0);
				currentRow = currentRow + 2;
				if(currentRow > 6){
					currentRow = 0;
				}
				printf("current row after moving down: %d\r\n", currentRow);
				OLED_pos(currentRow, 0);
				OLED_print('>');
				_delay_ms(1000);
			}
		}
	}
}

void select_item(int row){
	OLED_clear();
	printf("row %d\r\n", row);	
	if(row == 0){
		playing = 1;
		//start game
		start_animation("READY!");
		start_animation("SET...");
		start_animation("GO!!!!");
		OLED_pos(2,0);
		OLED_print_string("PING PONG TIME");
		start_game();
	}
	else if (row == 2){
		//change brightness
		OLED_pos(0,15);
		OLED_print_string("Brightness");
		OLED_pos(2,15);
		OLED_print_string("LOW");
		OLED_pos(4,15);
		OLED_print_string("MEDIUM");
		OLED_pos(6,15);
		OLED_print_string("HIGH");
		uint8_t x = adc_read(0);
		uint8_t y = adc_read(1);
		while(!(normalize_output_joystick(x, centerX) < -90)){
			x = adc_read(0);
			y = adc_read(1);
			if(normalize_output_joystick(y, centerY) > 90){
				OLED_clear_position(currentRow, 0);
				currentRow = currentRow - 2;
				if(currentRow < 0){
					currentRow = 6;
				}
				OLED_pos(currentRow, 0);
				OLED_print('>');
				_delay_ms(1000);
			}
			if(normalize_output_joystick(y, centerY) < -90){
				OLED_clear_position(currentRow, 0);
				currentRow = currentRow + 2;
				if(currentRow > 6){
					currentRow = 0;
				}
				OLED_pos(currentRow, 0);
				OLED_print('>');
				_delay_ms(1000);
			}
			if(!get_button_status()){
				select_brightness(currentRow);
			}
		}
	}
	else if (row == 4){
		//Instructions
		OLED_pos(0,0);
		OLED_print_string("0.Joy right:");
		OLED_pos(1,15);
		OLED_print_string("select item");
		OLED_pos(2,0);
		OLED_print_string("1.Start game");
		OLED_pos(3,0);
		OLED_print_string("2.Play");
		OLED_pos(4,0);
		OLED_print_string("3.If score:");
		OLED_pos(5,15);
		OLED_print_string("press joy");
		OLED_pos(6,0);
		OLED_print_string("4.If game over:");
		OLED_pos(7,15);
		OLED_print_string("reset");
	}
	else if (row == 6){
		//display center values
		char buffer[10];
		OLED_pos(0,15);
		OLED_print_string("Center values");
		printf("x: %d, y: %d\n\r", centerX, centerY);
		OLED_pos(2,15);
		OLED_print_string("X center: ");
		OLED_pos(3, 15);
		sprintf(buffer, "%d", centerX);
		OLED_print_string(buffer);
		OLED_pos(4,15);
		OLED_print_string("Y center: ");
		OLED_pos(5,15);
		sprintf(buffer, "%d", centerY);
		OLED_print_string(buffer);
	}
	navigate_menu();
}

void select_brightness(int row){
	if (row == 2){	//low brightness
		OLED_write_command(0x81);
		OLED_write_command(0x10);
	}
	else if (row == 4){	//medium brightness
		OLED_write_command(0x81);
		OLED_write_command(0x50);
	}
	else if (row == 6){	//high brightness
		OLED_write_command(0x81);
		OLED_write_command(0xa0);
	}
}

void start_animation(char *str){
	OLED_write_command(0x2E);
	OLED_write_command(0x29);	//set left scroll
	OLED_write_command(0x00);	//dummy byte
	OLED_write_command(0x00);	//start at page 0
	OLED_write_command(0x07);	//time interval of 2 frames
	OLED_write_command(0x07);	//end at page 7
	OLED_write_command(0x01);	//dummy byte
	OLED_pos(7, 0);
	OLED_print_string(str);
	OLED_write_command(0x2F);	//start animation
	_delay_ms(1000);
	OLED_write_command(0x2E);	//end animation
	_delay_ms(300);	
	OLED_clear();
}

void start_game(void){
	while(1){
		int valueH = adc_read(HORIZONTAL);
		int valueV = adc_read(VERTICAL);
		int button_status = get_button_status();
		send_joystick_status(valueH, valueV, button_status, centerX, centerY);
		_delay_ms(100);
	}
}