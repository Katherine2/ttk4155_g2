/*
 * delay.c
 *
 * Created: 08.11.2021 14:27:11
 *  Author: kathersi
 */ 

#include <stdint.h>
#include "sam.h"
#define US_TO_TICKS_CONVERTER (84 & SysTick_LOAD_RELOAD_Msk)		//https://www.at91.com/viewtopic.php?t=22349

static volatile uint16_t ticks;

void delay_us(uint16_t us) {
	ticks = us;
	SysTick_Config(1 * US_TO_TICKS_CONVERTER);
	
	while (ticks > 0);
}

void SysTick_Handler() {
	ticks--;
	if (ticks == 0) {
		SysTick->CTRL = 0;
	}
}