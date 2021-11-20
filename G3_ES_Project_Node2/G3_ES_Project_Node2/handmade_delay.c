/*
 * delay.c
 *
 * Created: 08.11.2021 14:27:11
 *  Author: kathersi
 */ 

#define F_CPU 84000000UL
#include "sam.h"

//using SysTick timer for the delay
void delay_ticks(int ticks)
{
	SysTick->LOAD = ticks;
	SysTick->CTRL = 1;
	
	while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
	SysTick->CTRL = 0;

}

void delay_us(int us)
{
	delay_ticks((us * (F_CPU / 8.4)) / 1000000);
}