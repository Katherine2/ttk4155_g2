/*
 * can.h
 *
 * Created: 04.10.2021 14:31:42
 *  Author: kathersi
 */ 


#ifndef CAN_H_
#define CAN_H_

typedef struct  
{
	int length;
	int idH;
	int idL;
	char data[];
}can_msg;

void can_init();
int flag_changed();



#endif /* CAN_H_ */