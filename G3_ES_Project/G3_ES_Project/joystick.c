/*
 * joystick.c
 *
 * Created: 22.09.2021 15:45:04
 *  Author: kathersi
 */ 

#include "adc.h"
#include "joystick.h"
#include "can.h"

//enum position{Left, Right, Up, Down, Neutral};

int joystick_init(uint8_t channel, int samples){
	uint8_t pos = 0;
	int center = 0;
	_delay_ms(100);
	for(int i=0; i<samples; i++){
		pos = adc_read(channel);
		printf("value: %d\r\n", pos);
		center = center + (int)pos;
	}
	//printf("sum: %d\r\n", center);
	center = center/samples;
	return center;
}

int normalize_output_joystick(uint8_t value, int center){
	int position = 0;
	if((int)value <= center){
		position = (((float)value/(float)center)*100)-100;
	}
	else if((int)value > center){
		position = ((((255-(float)value)/(255-(float) center))*(-100)))+100;
	}
	return position;
}

void send_calibration(int position, int id){
	//id 4 is horizontal, id 5 is vertical
	can_msg msg;
	msg.id = id;
	msg.length = 1;
	msg.data[0] = (char) position;
	can_transmit(msg);
}

int get_button_status(void){
	DDRD |= (0 << PD0);
	int status = PIND & 0x01;
	printf("Button status: %d\n\r", status);
	return status;
}

void send_position_horizontal(int position){
	//printf("position: %d\r\n", position);
	can_msg msg;
	msg.id = 1;		//id 1 for horizontal position
	msg.length = 1;
	msg.data[0] = (char)position;
	//printf("message id in node 1: %d\r\n", msg.id);
	//printf("message length in node 1: %d\r\n", msg.length);
	//printf("message data in node 1: %d\r\n\n", msg.data[0]);
	can_transmit(msg);
}

void send_position_vertical(int position){
	can_msg msg;
	msg.id = 2;		//id 2 for horizontal position
	msg.length = 1;
	msg.data[0] = (char)position;
	//printf("message id in node 1: %d\r\n", msg.id);
	//printf("message length in node 1: %d\r\n", msg.length);
	//printf("message data in node 1: %d\r\n\n", msg.data[0]);
	can_transmit(msg);
}

void send_button_status(int status){
	can_msg msg;
	msg.id = 3;		//id 3 for button
	msg.length = 1;
	msg.data[0] = (char)status;
	//printf("message id in node 1: %d\r\n", msg.id);
	//printf("message length in node 1: %d\r\n", msg.length);
	//printf("message data in node 1: %d\r\n\n", msg.data[0]);
	can_transmit(msg);
}