/*
 * motorbox.c
 *
 * Created: 03.11.2021 09:29:06
 *  Author: zahrajm
 */ 

#include "motorbox.h"
#include "sam.h"
#include "handmade_delay.h"

#define LEFT_CUTOFF	70
#define RIGHT_CUTOFF 130
//#define LEFT_CUTOFF	100
//#define RIGHT_CUTOFF 200

void dac_init(void){
	PMC->PMC_PCER1 |= PMC_PCER1_PID38; //enable clock for DACC
	REG_DACC_MR = DACC_MR_REFRESH(10) | DACC_MR_USER_SEL_CHANNEL1;
	REG_DACC_CHER = DACC_CHER_CH1;
	//REG_DACC_CDR //USE THIS TO SEND JOYSTICK POS
}

void motorbox_init(void){
	PIOC -> PIO_WPMR = (0x50494F << 8);	//disables write protect

	PMC->PMC_PCER0 |= (1 << ID_PIOC);//PMC_PCER0_PID13; //enable clock for 
	//PMC->PMC_PCER0 |= PMC_PCER0_PID14; //enable clock for 
	PMC->PMC_PCER0 |= (1 << ID_PIOD);
	
	//!OE
	PIOD -> PIO_PER = PIO_PD0;		//enables input/output function
	PIOD -> PIO_OER = PIO_PD0;		//enables output
	//SEL
	PIOD -> PIO_PER = PIO_PD2;		//enables input/output function
	PIOD -> PIO_OER = PIO_PD2;		//enables output
	//DIR
	PIOD -> PIO_PER = PIO_PD10;		//enables input/output function
	PIOD -> PIO_OER = PIO_PD10;		//enables output	
	//!RES
	PIOD -> PIO_PER = PIO_PD1;		//enables input/output function
	PIOD -> PIO_OER = PIO_PD1;		//enables output	
	//EN
	PIOD -> PIO_PER = PIO_PD9;		//enables input/output function
	PIOD -> PIO_OER = PIO_PD9;		//enables output
	/*
	//data bits
	PIOC -> PIO_PER = PIO_PC8;		//enables input/output function
	//PIOC -> PIO_ODR = PIO_PC8;		//disables output
	PIOC -> PIO_PER = PIO_PC7;		//enables input/output function
	//PIOC -> PIO_ODR = PIO_PC7;		//disables output
	PIOC -> PIO_PER = PIO_PC6;		//enables input/output function
	//PIOC -> PIO_ODR = PIO_PC6;		//disables output
	PIOC -> PIO_PER = PIO_PC5;		//enables input/output function
	//PIOC -> PIO_ODR = PIO_PC5;		//disables output
	PIOC -> PIO_PER = PIO_PC4;		//enables input/output function
	//PIOC -> PIO_ODR = PIO_PC4;		//disables output
	PIOC -> PIO_PER = PIO_PC3;		//enables input/output function
	//PIOC -> PIO_ODR = PIO_PC3;		//disables output
	PIOC -> PIO_PER = PIO_PC2;		//enables input/output function
	//PIOC -> PIO_ODR = PIO_PC2;		//disables output
	PIOC -> PIO_PER = PIO_PC1;		//enables input/output function
	//PIOC -> PIO_ODR = PIO_PC1;		//disables output
	*/
	PIOC -> PIO_IFER = (0xFF << 1);
	PIOC -> PIO_PUDR = (0xFF << 1);		//disables pull down register
	
	PIOD -> PIO_CODR = PIO_PD0;		//set !OE to low
	PIOD -> PIO_SODR = PIO_PD9;		//set EN to high
	PIOD -> PIO_CODR = PIO_PD1;		//set !RES to low
	delay_us(1000000);	
	PIOD -> PIO_SODR = PIO_PD1;		//set !RES to high
}

uint16_t receive_data(void){
	PIOD -> PIO_CODR = PIO_PD0;		//set !OE to low
	PIOD -> PIO_CODR = PIO_PD2;		//set SEL to low
	delay_us(1000000);					//wait 20 microseconds
	uint16_t msb = (PIOC->PIO_PDSR >> 1) & 0xFF ;	//read MSB
	PIOD -> PIO_SODR = PIO_PD2;		//set SEL to high
	delay_us(1000000);					//wait 20 microseconds
	uint16_t lsb = (PIOC->PIO_PDSR >> 1) & 0xFF ;	//read LSB
	PIOD -> PIO_SODR = PIO_PD0;		//set !OE to high
	return lsb | (msb << 8);
}
/*
void reset(void){
	//toggle !RST
	PIOD -> PIO_CODR = PIO_PD1;	
	PIOD -> PIO_SODR = PIO_PD1;
	//set !OE to high
	PIOD -> PIO_SODR = PIO_PD0;
}*/

void move_motor(int joystick_position, int center){
	if((joystick_position) < LEFT_CUTOFF){
		PIOD -> PIO_CODR = PIO_PD10;
		REG_DACC_CDR = 0xFFF - (joystick_position << 5);
	}
	else if ((joystick_position) > RIGHT_CUTOFF){
		PIOD -> PIO_SODR = PIO_PD10;
		REG_DACC_CDR = (joystick_position << 5) - 0xFFF;
	}
}