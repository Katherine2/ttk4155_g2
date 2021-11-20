/*
 * adc.c
 *
 * Created: 19.09.2021 12:17:07
 *  Author: kathersi
 */ 

#define ADC_BASE 0x1400			//address of the ADC

#include "adc.h"

volatile char *adc_reg = (char *) ADC_BASE;

//set the clock
void clock_timer() 
{
	//set fast PWM mode with non-inverted output
	DDRD |= (1<<PD4);  //set OC0 pin as output
	TCCR3A |= (1<<WGM32) | (1<<COM3A0);
	TCCR3B |= (1<<CS30) | (1<<WGM32);
	OCR3AH = 0b00000001;
}

uint8_t adc_read(uint8_t channel){	
	uint8_t adc_channel;
	
	//select which channel to write to
	if (channel==0){
		 adc_channel = 0b10000000;
	}
	else if (channel==1){
		adc_channel = 0b10000001;
	}
	else if (channel==2){
		adc_channel = 0b10000010;
	}
	else if (channel==3){
		adc_channel = 0b10000011;
	}
	adc_reg[0] = adc_channel;
	_delay_us(40);
	uint8_t adc_val = adc_reg[0];
	
	return adc_val;
}
