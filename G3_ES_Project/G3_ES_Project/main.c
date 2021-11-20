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
	
	fdevopen(USART_Transmit, USART_Receive);		//links the printf function
	
	clock_timer();
	SRAM_init();
	can_init();
	OLED_init();
	
	while (1){		
		menu_init();
	}
}


