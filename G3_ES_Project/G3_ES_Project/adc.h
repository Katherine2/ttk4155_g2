/*
 * adc.h
 *
 * Created: 19.09.2021 12:14:13
 *  Author: kathersi
 */ 

#include <avr/io.h>
#include <stdio.h>

void clock_timer();
void adc_init();
uint8_t adc_read(uint8_t channel);
