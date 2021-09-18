/*
 * ADJoystick.c
 *
 * Created: 15.09.2021 17:44:16
 * Author : kathersi
 */ 

#define F_CPU 4915200 // Clock Speed

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

void PWM_init();

int main(void) {
	PWM_init();
}

void PWM_init()
{
	/*set fast PWM mode with non-inverted output*/
	DDRD |= (1<<PD4);  /*set OC0 pin as output*/
	TCCR3A |= (1<<WGM32) | (1<<COM3A0);
	TCCR3B |= (1<<CS30) | (1<<WGM32);
	OCR3AH = 0b00000001;
}