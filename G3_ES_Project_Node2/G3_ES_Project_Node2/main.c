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
#include "motorbox.h"
#include "handmade_delay.h"

CAN_MESSAGE message;

int main(void) {
    // Initialize the SAM system 
    SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS;		//disable the watchdog timer
	configure_uart();
	can_init_def_tx_rx_mb(0x00290561);
	motorbox_init();
	dac_init();
	pwm_init();
	adc_init();
	pid_Init();
	delay_us(20);
	start_game();
	return 0;
}

