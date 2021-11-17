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
		
		//get centers
		centerH = (int)msg.data[3];
		centerV = (int)msg.data[4];
		
		//joystick horizontal position
		int new_data_H = normalize_position(msg.data[0], centerH);
		move_motor(new_data_H, centerH);
		
		//joystick vertical position
		int new_data_V = normalize_position(msg.data[1], centerV);
		move_servo(new_data_V);
		//joystick button
		button_pressed(msg.data[2]);
	}
	return msg;
}

void button_pressed(char d){
	PIOC -> PIO_PER = PIO_PC16;		//enables input/output function
	PIOC -> PIO_OER = PIO_PC16;		//sets pin PC16 (pin 47) as output
	PIOC -> PIO_PUDR = PIO_PC16;	//disables pull-ups
	if((int)d == 0){
		PIOC -> PIO_SODR = PIO_PC16;	//sets output data register
	}
	else{
		PIOC -> PIO_CODR = PIO_PC16;	//clear output data register
	}
}

int normalize_position(char d, int center){
		int value = (int)d;
		int position = 0;
		if((int)value <= center){
			position = (((float)value/(float)center)*100);
		}
		else if((int)value > center){
			position = ((((255-(float)value)/(255-(float) center))*(-100)))+200;
		}
		return position;
}

int calibrate_center(char d){
	int value, center;
	for (int i = 0; i < 10; i++){
		value = (int)d;
		center = center + value;
	}
	center = center/10;
	return center;
}