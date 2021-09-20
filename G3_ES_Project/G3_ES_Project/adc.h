/*
 * adc.h
 *
 * Created: 19.09.2021 12:14:13
 *  Author: kathersi
 */ 
#define F_CPU 4915200

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

void clock_timer();
void adc_init();
uint8_t adc_read(uint8_t channel);
