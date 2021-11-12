/*
 * menu.c
 *
 * Created: 04.10.2021 13:30:20
 *  Author: kathersi
 */ 

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
//static int prevPosX = 0;	//neutral

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
	OLED_print_string("Calibrate");
	currentRow = 0;
}

void navigate_menu(void){
	//clock_timer();
	if(!playing){
	printf("current row before while: %d\r\n", currentRow);
		while(1){
			uint8_t x = adc_read(0);
			uint8_t y = adc_read(1);
			printf("adc horizontal: %d, adc vertical: %d\n\r", x, y);
			//printf("x position: %d\r\n", normalize_output_joystick(x, centerX));
			if(normalize_output_joystick(x, centerX) < -90){
				//back
				//printf("left\r\n");
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
		OLED_pos(0,15);
		OLED_print_string("READY SET GO!");
		OLED_pos(2,15);
		OLED_print_string("PING PONG TIME");
		start_game();
		//break;
	}
	else if (row == 2){
		//change brightness
		OLED_pos(0,15);
		OLED_print_string("The sun");
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
		//calibrate joystick
		OLED_pos(0,15);
		OLED_print_string("Calibrating...");
		centerX = calibrate_joystick_center(HORIZONTAL, 10);
		centerY = calibrate_joystick_center(VERTICAL, 10);
		printf("x: %d, y: %d\n\r", centerX, centerY);
		OLED_pos(2,15);
		OLED_print_string("X center: ");
		OLED_pos(3, 15);
		OLED_print_string((char) centerX);
		OLED_pos(4,15);
		OLED_print_string("Y center: ");
		OLED_pos(5,15);
		OLED_print_string((char) centerY);
	}
	
	navigate_menu();
}

void start_game(void){
	//int centerH = calibrate_joystick_center(HORIZONTAL, 10);
	//int centerV = calibrate_joystick_center(VERTICAL, 10);
	while(1){
		//printf("center h: %d, center v: %d\n\r", centerH, centerV);
		int valueH = adc_read(HORIZONTAL);
		printf("adc horizontal value: %d\n\r", valueH);
		int valueV = adc_read(VERTICAL);
		int button_status = get_button_status();
		//printf("main function hdata: %d, vdata: %d, bstat: %d, hcenter:%d, vcenter:%d\n\r", valueH, valueV, button_status, centerX, centerY);
		send_joystick_status(valueH, valueV, button_status, centerX, centerY);
		_delay_ms(50);
	}
}