/*
 * GccApplication2.c
 *
 * Created: 06.09.2021 19:48:50
 * Author : amnaw
 */ 

#define F_CPU 4915200

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

int main(void){
	DDRA |= (1 << PA7);
	DDRE |= (1 << PE1);
	PORTA = 0x1;
	PORTE = 0x02;
	PORTA |= (1 << PA7);
	_delay_ms(1000);
	PORTE = 0x00;
	PORTA &= ~(1 << PA7);
	_delay_ms(100);
// 		
// 		_delay_ms(100);
}

