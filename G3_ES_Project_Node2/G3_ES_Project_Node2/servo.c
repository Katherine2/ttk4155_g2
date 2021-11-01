/*
 * servo.c
 *
 * Created: 27.10.2021 15:02:29
 *  Author: zahrajm
 */ 

#include "can_controller.h"
#include "joystick.h"
#include "sam.h"

#define PASSWD_PIO_PWM 0x50494F00

void pwm_init(void){
	PIOC -> PIO_PDR |= PIO_PDR_P18;		//enable peripheral control of the pin
	PIOC -> PIO_ABSR |= PIO_ABSR_P18;	//set it to peripheral B mode
	//need to clear WPEN bit in PMC Write Protect Register (we are assuming it defaults to 0)
	//PIOC -> PMC_PCER0 = PIO_PC18;
	PIOC->PIO_WPMR = PASSWD_PIO_PWM;	//disable write protection
	PMC->PMC_PCER1 |= (1 << 4);		//enable PWM clock

	//REG_PMC_PCER0 = 0x00002000;
	REG_PWM_CMR6 = 0xC;
	PWM->PWM_CLK = 0x00540000;
	REG_PWM_CPRD6 = 20000;//0x00004E20;
	//DUTY CYCLE
	REG_PWM_CDTY6 = 18000;//0x00002710; //need to vary this with joystick
	//REG_PWM_ENA = 0x00000040;

	PWM->PWM_ENA |= PWM_ENA_CHID6; // enable PWM channel 6
}

void move_servo(void){
	CAN_MESSAGE position;
	position = get_positions();
	move_to(position);
}

void move_to(char pos){
	//associate postion to pwm duty cycle
	//calls set_duty_cycle among other things
}

void set_duty_cycle(char dutyCycle){
	//calculates what value to set the REG_PWM_CDTY6 register to and then sets it to that value
	//the range of the CDTY6 must be between 18000 (2 ms duty cycle) and 19000 (1 ms duty cycle)
	
}