/*
 * adc_timer.c
 *
 * Created: 19.09.2021 12:17:07
 *  Author: kathersi
 */ 

#include "adc_timer.h"

void PWM_init()
{
	/*set fast PWM mode with non-inverted output*/
	DDRD |= (1<<PD4);  /*set OC0 pin as output*/
	TCCR3A |= (1<<WGM32) | (1<<COM3A0);
	TCCR3B |= (1<<CS30) | (1<<WGM32);
	OCR3AH = 0b00000001;
}