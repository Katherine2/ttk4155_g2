/*
 * menu.c
 *
 * Created: 04.10.2021 13:30:20
 *  Author: kathersi
 */ 

#include "joystick.h"
#include "oled.h"

static int centerX, centerY;
int currentRow;
//static int prevPosX = 0;	//neutral

void menu_init(){
	centerX = joystick_init(0, 10);
	centerY = joystick_init(1, 10);
	display_main_menu();
	navigate_menu();
}

void display_main_menu(){
	OLED_clear();
	OLED_pos(0,0);
	OLED_print('>');
	OLED_pos(0, 15);
	OLED_print_string("Start Game");
	OLED_pos(2, 15);
	OLED_print_string("Brightness");
	OLED_pos(4, 15);
	OLED_print_string("High Scores");
	OLED_pos(6, 15);
	OLED_print_string("Calibrate");
	currentRow = 0;
}

void navigate_menu(){
	clock_timer();
	printf("current row before while: %d\r\n", currentRow);
	while(1){
		uint8_t x = adc_read(0);
		uint8_t y = adc_read(1);
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
			//printf("current row after moving up: %d\r\n", currentRow);
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
			//printf("current row after moving down: %d\r\n", currentRow);
			OLED_pos(currentRow, 0);
			OLED_print('>');
			_delay_ms(1000);
		}
	}
}

void select_item(int row){
	OLED_clear();
	printf("row %d\r\n", row);	
	if(row == 0){
		//start game
		OLED_pos(0,15);
		OLED_print_string("Ping pong");
	}
	else if (row == 2){
		//change brightness
		OLED_pos(0,15);
		OLED_print_string("The sun");
	}
	else if (row == 4){
		//High score
		OLED_pos(0,15);
		OLED_print_string("You win!!");
	}
	else if (row == 6){
		//calibrate joystick
		OLED_pos(0,15);
		OLED_print_string("Calibrating...");
	}
	
	navigate_menu();
}