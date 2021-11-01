/*
 * joystick.c
 *
 * Created: 27.10.2021 14:42:18
 *  Author: zahrajm
 */
#include <stdio.h>
#include "can_controller.h"

CAN_MESSAGE msg;

CAN_MESSAGE get_positions(void){
	if(new_message_received()){
		msg = get_message();
		//print_message(msg);
		//printf("message data: %d \n\r", msg.data[0]);
	}
	return msg;
}
