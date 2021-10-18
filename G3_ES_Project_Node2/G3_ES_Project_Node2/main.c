/*
 * G3_ES_Project_Node2.c
 *
 * Created: 18.10.2021 14:07:15
 * Author : kathersi
 */ 


#define F_CPU 84000000
#include "sam.h"
#include <stdio.h>

#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	PIOA -> PIO_PER = PIO_PA19;		//enables input/output function
	PIOA -> PIO_OER = PIO_PA19;		//sets pin PA19 (pin 42) as output
	PIOA -> PIO_PUDR = PIO_PA19;	//disables pull-ups
	
	while (1) {
		//PIOA -> PIO_SODR = PIO_PA19;	//sets output data register
		//_delay_ms(500);
		PIOA -> PIO_CODR = PIO_PA19;	//clear output data register
	}
	
	/*
	configure_uart();
	
	while (1){
		//uart_putchar('!');
		UART_Handler();
		char c;
		uart_getchar(&c);
		printf("character received: %c\r\n", c);
	}*/
}
