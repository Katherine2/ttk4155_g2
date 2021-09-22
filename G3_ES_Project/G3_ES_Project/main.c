/*
 * G3_ES_Project.c
 *
 * Created: 19.09.2021 11:44:30
 * Author : kathersi
 */ 

#define F_CPU 4915200

#include "uart.h"
#include "sram.h"
#include "adc.h"
#include "joystick.h"
#include "slider.h"

#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


int main(void)
{	int rec;
	USART_Init(MYUBRR);
	
	fdevopen(USART_Transmit, USART_Receive);
	
	/* ADC */
	clock_timer();
	SRAM_init();
	
	int center = joystick_init(1, 10);
	//printf("center: %d\r\n", center);
	int norm_val = 0;
	while(1) {
		uint8_t v = adc_read(0);
		//printf("value: %d\r\n", v);
		norm_val = normalize_output_joystick(v, center);
		printf("normalized value: %d\r\n", norm_val);
		printf("value: %d\r\n\n", v);
	}
	
	
	
	/* SRAM in PastFiles	*/	
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
	
	/* UART in PastFiles
	
	while (1) {
		char c = USART_Receive();
		
		printf("Character received: %c\r\n", c);
	}*/
}


