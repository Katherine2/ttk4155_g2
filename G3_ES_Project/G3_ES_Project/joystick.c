/*
 * joystick.c
 *
 * Created: 22.09.2021 15:45:04
 *  Author: kathersi
 */ 

#include "adc.h"
#include "joystick.h"
#include "can.h"

//find the values of the center by averaging the first x smaples read by the joystick
//needs to be done to reduce the impact of the outliers
int calibrate_joystick_center(uint8_t channel, int samples){
	uint8_t pos = 0;
	int center = 0;
	_delay_ms(100);
	for(int i=0; i<samples; i++){
		pos = adc_read(channel);
		center = center + (int)pos;
	}
	center = center/samples;
	return center;
}

//normalizes the 0 to 255 data to -100 to 100
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

//read the value of the button
//button is active low
int get_button_status(void){
	DDRD |= (0 << PD0);
	int status = PIND & 0x01;
	return status;
}

//used for can to send the raw position of the joystick as well as the button status and the calculated centers
//raw position was used because the signed numbers were causing issues in the can structure
void send_joystick_status(int h_pos, int v_pos, int b_stat, int h_center, int v_center){
	can_msg msg;
	msg.id = 1;
	msg.length = 5;
	msg.data[0] = (char)h_pos;
	msg.data[1] = (char)v_pos;
	msg.data[2] = (char)b_stat;
	msg.data[3] = (char)h_center;
	msg.data[4] = (char)v_center;
	can_transmit(msg);
}