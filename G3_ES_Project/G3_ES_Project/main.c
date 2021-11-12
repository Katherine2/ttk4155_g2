/*
 * G3_ES_Project.c
 *
 * Created: 19.09.2021 11:44:30
 * Author : kathersi
 */ 

#define F_CPU 4915200
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include "uart.h"
#include "sram.h"
#include "adc.h"
#include "joystick.h"
#include "slider.h"
#include "oled.h"
#include "menu.h"
#include "can.h"
#include "spi.h"
#include "mcp2515.h"

#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#define HORIZONTAL 0
#define VERTICAL 1

int main(void){	
	USART_Init(MYUBRR);
	
	fdevopen(USART_Transmit, USART_Receive);
	
	clock_timer();
	SRAM_init();
	can_init();
	OLED_init();
	
	//int centerH = calibrate_joystick_center(HORIZONTAL, 10);
	//int centerV = calibrate_joystick_center(VERTICAL, 10);
	
	while (1){		
		menu_init();
		/*
		//printf("center h: %d, center v: %d\n\r", centerH, centerV);
		int valueH = adc_read(HORIZONTAL);
		//printf("adc horizontal value: %d\n\r", valueH);
		int valueV = adc_read(VERTICAL);
		int button_status = get_button_status();
		//printf("main function hdata: %d, vdata: %d, bstat: %d, hcenter:%d, vcenter:%d\n\r", valueH, valueV, button_status, centerH, centerV);
		send_joystick_status(valueH, valueV, button_status, centerH, centerV);
		_delay_ms(50);*/
	}
}


