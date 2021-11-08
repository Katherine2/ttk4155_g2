/*
 * motorbox.c
 *
 * Created: 03.11.2021 09:29:06
 *  Author: zahrajm
 */ 

#include "sam.h"
#include "handmade_delay.h"
/*
void dac_init(void){
	PMC->PMC_PCER1 |= PMC_PCER1_PID38; //enable clock for DACC
	REG_DACC_MR = DACC_MR_REFRESH(10) | DACC_MR_USER_SEL_CHANNEL1;
	REG_DACC_CHER = DACC_CHER_CH1;
	//REG_DACC_CDR //USE THIS TO SEND JOYSTICK POS
}*/

void motorbox_init(void){
	//need to do clock
	PMC->PMC_PCER0 |= PMC_PCER0_PID13; //enable clock for DACC
	PMC->PMC_PCER0 |= PMC_PCER0_PID14; //enable clock for DACC
	
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
	PIOC -> PIO_ODR = PIO_PC8;		//disables output
	PIOC -> PIO_PER = PIO_PC7;		//enables input/output function
	PIOC -> PIO_ODR = PIO_PC7;		//disables output
	PIOC -> PIO_PER = PIO_PC6;		//enables input/output function
	PIOC -> PIO_ODR = PIO_PC6;		//disables output
	PIOC -> PIO_PER = PIO_PC5;		//enables input/output function
	PIOC -> PIO_ODR = PIO_PC5;		//disables output
	PIOC -> PIO_PER = PIO_PC4;		//enables input/output function
	PIOC -> PIO_ODR = PIO_PC4;		//disables output
	PIOC -> PIO_PER = PIO_PC3;		//enables input/output function
	PIOC -> PIO_ODR = PIO_PC3;		//disables output
	PIOC -> PIO_PER = PIO_PC2;		//enables input/output function
	PIOC -> PIO_ODR = PIO_PC2;		//disables output
	PIOC -> PIO_PER = PIO_PC1;		//enables input/output function
	PIOC -> PIO_ODR = PIO_PC1;		//disables output
	*/
	
	PIOD -> PIO_CODR = PIO_PD0;		//set !OE to low
	PIOD -> PIO_SODR = PIO_PD0;		//set EN to high
	PIOD -> PIO_CODR = PIO_PD1;		//set !RES to low
	delay_us(30);	
	PIOD -> PIO_SODR = PIO_PD1;		//set !RES to high
}

uint16_t receive_data(void){
	//set SEL to low
	PIOD -> PIO_CODR = PIO_PD2;		//set SEL to low
	//wait 20 microseconds
	delay_us(30);
	//read MSB
	uint16_t msb = (PIOD->PIO_PDSR & 0xFF) >> 1;
	//set SEL to high
	PIOD -> PIO_SODR = PIO_PD2;
	//wait 20 microseconds
	delay_us(30);
	//read LSB
	uint16_t lsb = (PIOD->PIO_PDSR & 0xFF) >> 1;
	
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