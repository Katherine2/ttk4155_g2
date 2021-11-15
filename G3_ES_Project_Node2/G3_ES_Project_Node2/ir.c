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
	PMC->PMC_PCER1 |= PMC_PCER1_PID37;//(1 << 6);		//enable ADC clock
	//ADC->ADC_WPMR = PASSWD_PIO_ADC;	//disable write protection
	/*
	USEQ = 0 => normal mode (because we don't know otherwise)
	TRANSFER = 01 => 1 (because we didn't know and p.1408 said that)
	TRACKTIM = 0000 => 0 (because we didn't know and p.1408 said that)
	ANACH = 0 => no analog change (because we don't know otherwise)
	SETTLING = 00 => 3 periods of ADCClock (because we don't know otherwise)
	STARTUP = 0000 => 0 periods of ADCClock (because we don't know otherwise)
	PRESCAL = 10000000 => 128 gives adc clock of about 325kHz (picked it at random to fit in the 8 bits)
	FREERUN = 1 => Free Run Mode (because TA said in lecture)
	FWUP = 0 => Normal Mode (because we don't know otherwise)
	SLEEP = 0 => Normal Mode (because we don't know otherwise)
	LOWRES = 0 => 12-bit resolution (because it makes more sense)
	TRGSEL = 000 => don't care about the value since TRGEN is 0
	TRGEN = 0 => Hardware triggers are disabled (because we don't know otherwise)
	*/
	ADC->ADC_MR |= ADC_MR_FREERUN_ON;//0b00010000000000001000000010000000;
	ADC->ADC_CHER |= ADC_CHER_CH0;//0x00000001;		//enables channel 0
	ADC->ADC_CR |= ADC_CR_START;//0x00000002;		//starts analog-to-digital conversion
	//ADC->ADC_CDR; //the converted data is stored here for a specific channel
	//ADC->ADC_LCDR; //the last converted data of all channels is stored here with the channel number until a new conversion is complete
	//channel and pin numbre are inverted, that is pin A0 is channel 7 and channel 0 is pin A7
}

int adc_read(void){
	//printf("adc value: %d\n\r", (int)ADC->ADC_CDR[0]);
	return ADC->ADC_CDR[0];
}

int is_goal(int value, int score){
	//printf("value: %d\n\r", value);
	if(value < IR_THRESHOLD){
		score++;
		pause_game(score);
	}
	//printf("score: %d\n\r", score);
	return score;
}

