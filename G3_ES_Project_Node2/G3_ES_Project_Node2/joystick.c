/*
 * joystick.c
 *
 * Created: 27.10.2021 14:42:18
 *  Author: zahrajm
 */
#include <stdio.h>
#include "can_controller.h"
#include "sam.h"

CAN_MESSAGE msg;

CAN_MESSAGE get_positions(void){
	
	if(new_message_received()){
		msg = get_message();
		if(msg.id == 1){
			//joystick horizontal position
			return msg;
		}
		else if(msg.id == 2){
			//joystick vertical position
			move_servo(msg);
			return msg;
		}
		else if(msg.id == 3){
			//joystick button
			button_pressed(msg);
			return msg;
		}
		//print_message(msg);
		//printf("message data: %d \n\r", msg.data[0]);
}
return msg;
}

void button_pressed(CAN_MESSAGE message){
	
	//printf("button\n\r");
	PIOC -> PIO_PER = PIO_PC16;		//enables input/output function
	PIOC -> PIO_OER = PIO_PC16;		//sets pin PC16 (pin 47) as output
	PIOC -> PIO_PUDR = PIO_PC16;	//disables pull-ups
	if(message.data[0] == 0){
		printf("button pressed\n\r");
		PIOC -> PIO_SODR = PIO_PC16;	//sets output data register
	}
	else{
		PIOC -> PIO_CODR = PIO_PC16;	//clear output data register
	}
}