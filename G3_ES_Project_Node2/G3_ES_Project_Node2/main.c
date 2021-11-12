/*
 * G3_ES_Project_Node2.c
 *
 * Created: 18.10.2021 14:07:15
 * Author : kathersi
 */ 


#define F_CPU 84000000

#include <stdio.h>
#include <stdint.h>

#include "sam.h"
#include "can_controller.h"
#include "joystick.h"
#include "servo.h"
//#include "ir.h"
#include "motorbox.h"
#include "handmade_delay.h"

//#define PASSWD_PIO_PWM 0x50494F00

/*
#define FOSC 16000000 // Clock Speed
#define CANBAUD 125000
#define BRP 3
#define TQ (2*(BRP+1))/FOSC		//500ns
#define BITTIME 1/(TQ*CANBAUD)	//13TQ => SyncSeg = 1, PropSeg = 1, PS1 = 6, PS2 = 5, SJW = 1
*/

CAN_MESSAGE message;

int main(void) {
    /* Initialize the SAM system */
    SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS;		//disable the watchdog timer
	configure_uart();
	can_init_def_tx_rx_mb(0x00290561);
	motorbox_init();
	dac_init();
	pwm_init();
	adc_init();
	delay_us(1000000);
	start_game();
	/*while(1) {
		//adc = adc_read();
		//printf("ir value: %d\n\r", adc);
		//goal = is_goal(adc, goal);
		//message = get_positions();
		//uint16_t data = receive_data();
		//printf("data: %d\n\r", data);
		//delay_us(1000000000);
	}
	return 0;*/
}

