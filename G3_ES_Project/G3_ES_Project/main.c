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

int main(void)
{	int rec;
	USART_Init(MYUBRR);
	
	fdevopen(USART_Transmit, USART_Receive);
	
	clock_timer();
	SRAM_init();
	can_init();
	
	//int centerH = joystick_init(HORIZONTAL, 10);
	//printf("CENTER: %d\r\n", centerH);
	
	//int centerV = joystick_init(VERTICAL, 10);
	//int outputH = 0;
	/*
	while(1){
		//uint8_t valueH = adc_read(HORIZONTAL);
		//printf("ADC output X: %d\r\n", valueH);
		//outputH = normalize_output_joystick(valueH, centerH);
		//printf("Horizontal output: %d\r\n", outputH);
		int valueV = adc_read(VERTICAL);
		printf("ADC output Y: %d\r\n", valueV);
		int outputV = normalize_output_joystick(valueV, centerV);
		//printf("Vertical output: %d\r\n", outputV);
	}*/
	//***************** SENDING JOYSTICK POSITIONS OVER CAN TO NODE 2 **************/
	
	while (1){
		int valueH = adc_read(HORIZONTAL);
		//printf("ADC output X: %d\r\n", valueH);
		//int valueV = adc_read(VERTICAL);
		//printf("ADC output Y: %d\r\n\n", valueV);
		send_position(valueH/*normalize_output_joystick(valueH, centerH)*/);
		//send_position(valueV/*normalize_output_joystick(valueV, centerV)*/);
		_delay_ms(1000);
	}
	/*********************************** CAN **************************************/
	/*
	can_init();
	can_msg a;
	a.id = 2;
	a.length = 3;
	a.data[0] = '#';
	while(1) {
		//a.data[0] = a.data[0]+1;
		can_transmit(a);
		_delay_ms(1000);
	}
	*/
	/********************************** OLED ***********************************/
	/*
	SRAM_init();
	OLED_init();
	
	menu_init();
	*/
	
	/********************************** ADC **********************************/
	/* 
	clock_timer();
	SRAM_init();
	
	printf("center: %d\r\n", center);
	int norm_val = 0;
	while(1) {
		uint8_t v = adc_read(0);
		//printf("value: %d\r\n", v);
		norm_val = normalize_output_joystick(v, center);
		printf("normalized value: %d\r\n", norm_val);
		printf("value: %d\r\n\n", v);
	}
	*/
	
	/********************************* SRAM *************************************/
	//SRAM_init();
	//SRAM_test();	
	
	
	/* Latch in PastFiles
	DDRA |= (1 << PA7);
	DDRE |= (1 << PE1);
	PORTA = 0x1;
	PORTE = 0x02;
	PORTA |= (1 << PA7);
	_delay_ms(1000);
	PORTE = 0x00;
	PORTA &= ~(1 << PA7);
	_delay_ms(100);
	*/
	
	/*********************************** UART **************************************/
	/* 
	while (1) {
		char c = USART_Receive();
		
		printf("Character received: %c\r\n", c);
	}*/
}


