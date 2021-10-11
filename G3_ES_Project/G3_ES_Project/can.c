/*
 * can.c
 *
 * Created: 04.10.2021 14:31:30
 *  Author: kathersi
 */ 

//High level sending and receiving CAN messages

#include "can.h"
#include "mcp2515.h"
#include <stdint.h>

static int flag;


void can_init(){
	mcp2515_init();
	mcp2515_bit_modify(MCP_CANINTE, 0X01, 0x01);		//enables the transmission complete flag (for the interrupt)
	mcp2515_write(MCP_CANCTRL, MODE_LOOPBACK);			//setting the mode to loopback for now
}

int flag_changed(){
	return flag;
}

void can_receive(){
	while(!flag_changed()){};
	mcp2515_read_status();
	//length = mcp2515_read()
	
}

void can_transmit(can_msg msg){
	mcp2515_write(MCP_TXB0SIDH, msg.idH);
	mcp2515_write(MCP_TXB0SIDL, msg.idL);
	mcp2515_write(MCP_TXB0DLC, msg.length);
	mcp2515_write(MCP_TXB0Dm, msg.data);
	mcp2515_request_to_send();
}


ISR(INT0_vect){
	flag = 1;	
}




