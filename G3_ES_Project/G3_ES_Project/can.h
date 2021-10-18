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
	int id;
	char data[8];
	
}can_msg;

void can_init(void);
void can_transmit(can_msg msg);
void can_receive(void);

#endif /* CAN_H_ */