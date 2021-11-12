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
#define MAX_BOTTOM_CUTOFF	30
#define MAX_TOP_CUTOFF 170
#define BOTTOM_CUTOFF	70
#define TOP_CUTOFF 130
#define COMPLETE_LEFT 2.0
#define LEFT 1.75
#define CENTER 1.5
#define RIGHT 1.25
#define COMPLETE_RIGHT 1.0

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

/*
void move_servo(int joystick_position){
	//CAN_MESSAGE position;
	//position = get_positions();
	//printf("position: %d \n\r", joystick_position);
	move_to(joystick_position);
}
*/

void move_servo(int pos){
	//associate position to pwm duty cycle
	//calls set_duty_cycle among other things
	int dty = 20000 - 1000*CENTER;
	if (pos < MAX_BOTTOM_CUTOFF){
		dty = 20000 - 1000*COMPLETE_LEFT;
	}
	else if ((MAX_BOTTOM_CUTOFF < pos) && (pos < BOTTOM_CUTOFF)){
		dty = 20000 - 1000*LEFT;
	}
	else if ((BOTTOM_CUTOFF < pos) && (pos < TOP_CUTOFF)){
		dty = 20000 - 1000*CENTER;
	}
	else if ((TOP_CUTOFF < pos) && (pos < MAX_TOP_CUTOFF)){
		dty = 20000 - 1000*RIGHT;
	}
	else if (pos > MAX_TOP_CUTOFF) {
		dty = 20000 - 1000*COMPLETE_RIGHT;
	}
	else{
		printf("ERROR! Invalid position");
	}
	set_duty_cycle(dty);
}

void set_duty_cycle(int dty){
	//calculates what value to set the REG_PWM_CDTY6 register to and then sets it to that value
	//the range of the CDTY6 must be between 18000 (2 ms duty cycle) and 19000 (1 ms duty cycle)
	//printf("cycle: %d\n\r", dutyCycle);
	//int dty = 20000 - 1000*dutyCycle;
	//printf("duty cycle: %d\n\r", dty);
	if((MAX_DUTY_CYCLE <= dty) && (dty <= MIN_DUTY_CYCLE)){
		REG_PWM_CDTY6 = dty;
	}
	else{
		printf("ERROR! Duty cycle out of range. Must be between 1 and 2\n\r");
	}
}