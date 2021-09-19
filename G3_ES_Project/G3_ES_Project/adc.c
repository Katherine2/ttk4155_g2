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
	
}

//volatile
uint8_t adc_read(uint8_t channel){
	
}