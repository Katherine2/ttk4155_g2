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

int calibrate_joystick_center(uint8_t channel, int samples){
	uint8_t pos = 0;
	int center = 0;
	_delay_ms(100);
	for(int i=0; i<samples; i++){
		pos = adc_read(channel);
		//printf("value: %d\r\n", pos);
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

/*
void send_calibration(int position, int data_array_pos){
	//data_array_pos 3 is horizontal, data_array_pos 4 is vertical
	can_msg msg;
	msg.id = 1;
	msg.length = 1;
	msg.data[data_array_pos] = (char) position;
	can_transmit(msg);
}
*/
int get_button_status(void){
	DDRD |= (0 << PD0);
	int status = PIND & 0x01;
	//printf("Button status: %d\n\r", status);
	return status;
}
/*
void send_position_horizontal(int position){
	//printf("position: %d\r\n", position);
	can_msg msg;
	msg.id = 1;		
	msg.length = 1;
	msg.data[0] = (char)position; //data[0] for horizontal position
	//printf("message id in node 1: %d\r\n", msg.id);
	//printf("message length in node 1: %d\r\n", msg.length);
	//printf("message data in node 1: %d\r\n\n", msg.data[0]);
	can_transmit(msg);
}

void send_position_vertical(int position){
	can_msg msg;
	msg.id = 1;		
	msg.length = 1;
	msg.data[1] = (char)position; //data[1] for horizontal position
	//printf("message id in node 1: %d\r\n", msg.id);
	//printf("message length in node 1: %d\r\n", msg.length);
	//printf("message data in node 1: %d\r\n\n", msg.data[0]);
	can_transmit(msg);
}

void send_button_status(int status){
	can_msg msg;
	msg.id = 1;		
	msg.length = 1;
	msg.data[2] = (char)status;	//data[2] for button
	//printf("message id in node 1: %d\r\n", msg.id);
	//printf("message length in node 1: %d\r\n", msg.length);
	//printf("message data in node 1: %d\r\n\n", msg.data[0]);
	can_transmit(msg);
}
*/
void send_joystick_status(int h_pos, int v_pos, int b_stat, int h_center, int v_center){
	//printf("hdata: %d, vdata: %d, bstat: %d, hcenter:%d, vcenter:%d\n\r", h_pos, v_pos, b_stat, h_center, v_center);
	can_msg msg;
	msg.id = 1;
	msg.length = 5;		//does length need to be increased to 3?
	msg.data[0] = (char)h_pos;
	msg.data[1] = (char)v_pos;
	msg.data[2] = (char)b_stat;
	msg.data[3] = (char)h_center;
	msg.data[4] = (char)v_center;
	can_transmit(msg);
}