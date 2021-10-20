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

void send_position(int position){
	printf("position: %d\r\n", position);
	can_msg msg;
	msg.id = 1;
	msg.length = 1;
	msg.data[0] = (char)position;
	can_transmit(msg);
}