/*
 * delay.c
 *
 * Created: 08.11.2021 14:27:11
 *  Author: kathersi
 */ 

#define F_CPU 84000000UL
#include "sam.h"


/* Function for performing delay by using SysTick */
void delay_ticks(int ticks)
{
	SysTick->LOAD = ticks;
	SysTick->CTRL = 1;
	
	// COUNTFLAG is a bit that is set to 1 when counter reaches 0.
	// It's automatically cleared when read.
	while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
	SysTick->CTRL = 0;

}

/* Function for implementing us delay */
void delay_us(int us)
{
	delay_ticks((us * (F_CPU / 8.4)) / 1000000);
}


/* Function for implementing ms delay */
void delay_ms(int ms)
{
	delay_ticks((ms * (F_CPU / 8.4)) / 1000);
}