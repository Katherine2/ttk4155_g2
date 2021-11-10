/*
 * joystick.c
 *
 * Created: 27.10.2021 14:42:18
 *  Author: zahrajm
 */
#include <stdio.h>
#include "joystick.h"
#include "can_controller.h"
#include "sam.h"
#include "motorbox.h"
#include "servo.h"

CAN_MESSAGE msg;
int centerV, centerH;

CAN_MESSAGE get_positions(void){
	if(new_message_received()){
		msg = get_message();
		print_message(msg);
		if(msg.id == 1){
			//joystick horizontal position
			int new_data = normalize_position(msg, centerH);
			printf("center: %d, new data: %d\n\r", centerH, new_data);
			move_motor(new_data, 100);
			return msg;
		}
		else if(msg.id == 2){
			//joystick vertical position
			int new_data = normalize_position(msg, centerV);
			//printf("center: %d, new data: %d\n\r", centerH, new_data);
			move_servo(new_data);
			return msg;
		}
		else if(msg.id == 3){
			//joystick button
			button_pressed(msg);
			return msg;
		}
		else if(msg.id == 4){
			//horizontal calibration
			centerH = calibrate_center(msg);
			printf("center H: %d\n\r", centerH);
		}
		else if(msg.id == 5){
			//vertical calibration
			centerV = calibrate_center(msg);
			printf("center V: %d\n\r", centerV);
		}
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

int normalize_position(CAN_MESSAGE message, int center){
		int value = (int)message.data[0];
		int position = 0;
		if((int)value <= center){
			position = (((float)value/(float)center)*100);
		}
		else if((int)value > center){
			position = ((((255-(float)value)/(255-(float) center))*(-100)))+200;
		}
		return position;
}

int calibrate_center(CAN_MESSAGE message){
	int value, center;
	for (int i = 0; i < 10; i++){
		value = (int)message.data[0];
		center = center + value;
	}
	center = center/10;
	return center;
}