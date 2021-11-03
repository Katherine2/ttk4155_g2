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
#define MIN_DUTY_CYCLE 19000
#define MAX_DUTY_CYCLE 18000
#define LEFT_CUTOFF	100
#define RIGHT_CUTOFF 200
#define LEFT 1.0
#define CENTER 1.5
#define RIGHT 2.0

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
	//int dty = 20000 - 1000*3;
	//REG_PWM_CDTY6 = dty;
	//REG_PWM_CDTY6 = 10000;
	//REG_PWM_CDTY6 = 10000;//0x00002710; //need to vary this with joystick
	//REG_PWM_ENA = 0x00000040;

	PWM->PWM_ENA |= PWM_ENA_CHID6; // enable PWM channel 6
}

void move_servo(void){
	CAN_MESSAGE position;
	position = get_positions();
	printf("position: %d \n\r", position.data[0]);
	move_to(position.data[0]);
}

void move_to(char pos){
	//associate postion to pwm duty cycle
	//calls set_duty_cycle among other things
	//if the joystick is towards the left, set the duty cycle to 1.0
	//printf("pos: %d \n\r", (int)pos);	
	if ((int)pos < LEFT_CUTOFF){
		//printf("left\n\r");
		set_duty_cycle(RIGHT);
	}
	//if the joystick is in the center, set the duty cycle to 1.5
	else if ((LEFT_CUTOFF < (int)pos) && ( (int)pos < RIGHT_CUTOFF)){
		//printf("center \n\r");
		set_duty_cycle(CENTER);
	}
	//if the joystick is towards the right, set the duty cycle to 2.0
	else if (RIGHT_CUTOFF < (int)pos) {
		//printf("right\n\r");
		set_duty_cycle(LEFT);
	}
	else{
		printf("ERROR! Invalid position");
	}
	//if the joystick is in the center, set the duty cycle to 1.5
}

void set_duty_cycle(float dutyCycle){
	//calculates what value to set the REG_PWM_CDTY6 register to and then sets it to that value
	//the range of the CDTY6 must be between 18000 (2 ms duty cycle) and 19000 (1 ms duty cycle)
	//printf("cycle: %f\n\r", dutyCycle);
	int dty = 20000 - 1000*dutyCycle;
	//printf("duty cycle: %d\n\r", dty);
	if((MAX_DUTY_CYCLE <= dty) && (dty <= MIN_DUTY_CYCLE)){
		REG_PWM_CDTY6 = dty;
	}
	else{
		printf("ERROR! Duty cycle out of range. Must be between 1 and 2/n/r");
	}
}