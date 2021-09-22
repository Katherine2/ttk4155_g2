/*
 * slider.c
 *
 * Created: 22.09.2021 16:42:33
 *  Author: kathersi
 */ 

#include "adc.h"
#include "slider.h"

int normalize_output_slider(uint8_t value){
	int position = 0;
	position = (float)value/2.55;
	
	return position;
}