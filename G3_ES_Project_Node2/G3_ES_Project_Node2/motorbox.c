/*
 * motorbox.c
 *
 * Created: 03.11.2021 09:29:06
 *  Author: zahrajm
 */ 

#include <stdio.h>
#include <stdlib.h>
#include "motorbox.h"
#include "sam.h"
#include "handmade_delay.h"

#define LEFT_CUTOFF	50
#define RIGHT_CUTOFF 150

void dac_init(void){
	PMC->PMC_PCER1 |= PMC_PCER1_PID38; //enable clock for DACC
	//REG_DACC_MR = DACC_MR_REFRESH(10) | DACC_MR_USER_SEL_CHANNEL1;
	REG_DACC_MR = DACC_MR_USER_SEL_CHANNEL1;
	REG_DACC_CHER |= DACC_CHER_CH1;
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

	PIOC -> PIO_IFER = (0xFF << 1);
	PIOC -> PIO_PUDR = (0xFF << 1);		//disables pull down register
	
	PIOD -> PIO_CODR = PIO_PD0;		//set !OE to low
	PIOD -> PIO_SODR = PIO_PD9;		//set EN to high
	PIOD -> PIO_CODR = PIO_PD1;		//set !RES to low
	delay_us(1000000);	
	PIOD -> PIO_SODR = PIO_PD1;		//set !RES to high
}

int16_t receive_data(void){
	int16_t data;
	PIOD -> PIO_CODR = PIO_PD0;		//set !OE to low
	PIOD -> PIO_CODR = PIO_PD2;		//set SEL to low
	delay_us(1000000);					//wait 20 microseconds
	data |= ((PIOC->PIO_PDSR >> 1) & 0xFF) << 8 ;	//read MSB
	PIOD -> PIO_SODR = PIO_PD2;		//set SEL to high
	delay_us(1000000);					//wait 20 microseconds
	data |= (PIOC->PIO_PDSR >> 1) & 0xFF ;	//read LSB
	PIOD -> PIO_SODR = PIO_PD0;		//set !OE to high
	return data; //lsb | (msb << 8);
}

int calibrate_motor(int16_t position_from_motor){
	int new_position;
	new_position = abs(((position_from_motor/40)*0.91)-200);
	//printf("upper layer motor data: %d\n\r", new_position);
	return new_position;
}

void move_motor(int joystick_position, int center){
	printf("joy stick: %d\n\r", joystick_position);
	//printf("center: %d\n\rpos: %d\n\r", center);

	int16_t motor_data = receive_data();
	int calibrated_motor_data = calibrate_motor(motor_data);
	int16_t pid_output = pid_Controller(joystick_position, calibrated_motor_data);
	printf("Calib Motor Data : %d\n\r", calibrated_motor_data);
	printf("PID : %d\n\r", pid_output);

	//REG_DACC_CDR = (pid_output);

	//DACC->DACC_CDR = DACC_CDR_DATA(pid_output*85);
	//printf("DACC CDR DATA: %d \n\r", DACC_CDR_DATA(pid_output*85));
	
	DACC->DACC_CDR = (1 & 0b11) << 12 | DACC_CDR_DATA(pid_output*100);
	//printf("DACC CDR DATA: %d \n\r", DACC_CDR_DATA(pid_output*100));
	printf("Output to register: %d \n\r", (1 & 0b11) << 12 | DACC_CDR_DATA(pid_output*100));
	
	
	if(joystick_position <= calibrated_motor_data){
		printf("left: %d\n\r", pid_output);
		PIOD -> PIO_CODR = PIO_PD10;
		//REG_DACC_CDR = (0xFFF -(pid_output) )  ;
		//DACC->DACC_CDR = (1 & 0b11) << 12 | DACC_CDR_DATA(pid_output*100);
		//printf("DACC CDR DATA: %d \n\r", DACC_CDR_DATA(pid_output*100));
		//printf("Output to register: %d \n\r", (1 & 0b11) << 12 | DACC_CDR_DATA(pid_output*100));
	}
	else if ((joystick_position) > calibrated_motor_data){
		printf("right: %d\n\r", pid_output);
		PIOD -> PIO_SODR = PIO_PD10;
		//REG_DACC_CDR = (0xFFF -(pid_output) ) ;
		//DACC->DACC_CDR = (1 & 0b11) << 12 | DACC_CDR_DATA(pid_output*100);
		//printf("DACC CDR DATA: %d \n\r", DACC_CDR_DATA(pid_output*100));
		//printf("Output to register: %d \n\r", (1 & 0b11) << 12 | DACC_CDR_DATA(pid_output*100));
	}
	//else{
		//REG_DACC_CDR = pid_output;
	//}
}