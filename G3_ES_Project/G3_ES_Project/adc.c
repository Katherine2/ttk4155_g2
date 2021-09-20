/*
 * adc.c
 *
 * Created: 19.09.2021 12:17:07
 *  Author: kathersi
 */ 

#include "adc.h"

//set the clock
void clock_timer() //previously called PWM_init() but I feel like this makes more sense as a function name
{
	/*set fast PWM mode with non-inverted output*/
	DDRD |= (1<<PD4);  /*set OC0 pin as output*/
	TCCR3A |= (1<<WGM32) | (1<<COM3A0);
	TCCR3B |= (1<<CS30) | (1<<WGM32);
	OCR3AH = 0b00000001;
}


void adc_init(){
	//setting the ADC CS to low to select it (active low) from the NAND gates
	DDRC |= (1 << PC2);
	PORTC = 0x04;
	//sets read and write as outputs
	DDRD |= (1 << PD7) | (1 << PD6);
}

//volatile
uint8_t adc_read(uint8_t channel){
	//set the pin as outputs
	DDRA |= (1 << PA0) | (1 << PA1);
	//select which channel to write to
	if (channel==0){
		PORTA |= (0 << PA0) | (0 << PA1);
	}
	else if (channel==1){
		PORTA |= (1 << PA0) | (0 << PA1);
	}
	else if (channel==2){
		PORTA |= (0 << PA0) | (1 << PA1);
	}
	else if (channel==3){
		PORTA |= (1 << PA0) | (1 << PA1);
	}
	//set read to 1 and write to 0 so that we can write
	PORTD = 0x80;
	_delay_us(4);
	DDRA = 0x00;
	//set write to 1 and read to 0 so that we can read
	PORTD = 0x40;

}
