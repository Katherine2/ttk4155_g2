/*
 * G3_ES_Project.c
 *
 * Created: 19.09.2021 11:44:30
 * Author : kathersi
 */ 

#define F_CPU 4915200

#include "uart.h"
#include "sram.h"
#include "adc_timer.h"

#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


int main(void)
{	
	USART_Init(MYUBRR);
	
	fdevopen(USART_Transmit, USART_Receive());
	/*	
	SRAM_init();
	SRAM_test();	
	*/
	
	while (1) {
		char c = USART_Receive();
		
		printf("Character received: %c\r\n", c);
	}
}



