/*
 * ir.c
 *
 * Created: 27.10.2021 15:13:47
 *  Author: zahrajm
 */ 

#include <stdio.h>
#include "sam.h"

#define PASSWD_PIO_ADC 0x41444300
#define IR_THRESHOLD 300

void adc_init(void){
	PMC->PMC_PCER1 |= PMC_PCER1_PID37;	//enable ADC clock
	ADC->ADC_MR |= ADC_MR_FREERUN_ON;
	ADC->ADC_CHER |= ADC_CHER_CH0;		//enables channel 0
	ADC->ADC_CR |= ADC_CR_START;		//starts analog-to-digital conversion
	//ADC->ADC_CDR; //the converted data is stored here for a specific channel
	//ADC->ADC_LCDR; //the last converted data of all channels is stored here with the channel number until a new conversion is complete
	//channel and pin numbre are inverted, that is pin A0 is channel 7 and channel 0 is pin A7
}

int adc_read(void){
	return ADC->ADC_CDR[0];
}

int is_goal(int value, int score){
	PIOA -> PIO_PER = PIO_PA19;			//enables input/output function
	PIOA -> PIO_OER = PIO_PA19;			//sets pin PA19 (pin 42) as output
	PIOA -> PIO_PUDR = PIO_PA19;		//disables pull-ups
	if(value < IR_THRESHOLD){
		score++;
		PIOA -> PIO_SODR = PIO_PA19;	//turn on LED
		pause_game(score);
	}
	printf("score: %d\n\r", score);
	return score;
}

