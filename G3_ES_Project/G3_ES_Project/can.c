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
#include <avr/interrupt.h>
//static int* flag_ptr;
static int flag;
//flag_ptr = &flag;


void can_init(){
	mcp2515_init();
	mcp2515_bit_modify(MCP_CANINTE, 0X03, 0x03);		//enables the reception complete flag (for the interrupt)
	mcp2515_write(MCP_CANCTRL, MODE_LOOPBACK);			//setting the mode to loopback for now
	// Disable global interrupts
	cli() ;
	// Interrupt on rising edge
	EMCUCR |= (1<<ISC00);
	// Enable interrupt 
	GICR |= (1<<INT0);
	// Enable global interrupts
	sei() ;
	}

int flag_changed(){
	return flag;
	//return *flag_ptr;
}

void can_receive(){
	printf("before flag\r\n");
	if(flag_changed()){	
	printf("after flag\r\n");
		if(mcp2515_read_status() & 0x01){
			//printf("in can receive");
			//mcp2515_read_status();
			//length = mcp2515_read()
			can_msg msg;
			msg.idH = mcp2515_read(MCP_RXB0SIDH);
			msg.idL = mcp2515_read(MCP_RXB0SIDL);
			msg.length = mcp2515_read(MCP_RXB0DLC);
			//msg.data = mcp2515_read_array(MCP_RXB0Dm, msg.length);
			mcp2515_read_array(MCP_RXB0Dm, msg.length);
			//printf(msg.data);
			mcp2515_bit_modify(MCP_CANINTF,0x01, 0x00);
		}
		
		if(mcp2515_read_status() & 0x02){
			//printf("in can receive");
			//mcp2515_read_status();
			//length = mcp2515_read()
			can_msg msg;
			msg.idH = mcp2515_read(MCP_RXB1SIDH);
			msg.idL = mcp2515_read(MCP_RXB1SIDH);
			msg.length = mcp2515_read(MCP_RXB1DLC);
			//msg.data = mcp2515_read_array(MCP_RXB1Dm, msg.length);
			mcp2515_read_array(MCP_RXB1Dm, msg.length);
			//printf(msg.data);
			mcp2515_bit_modify(MCP_CANINTF,0x02, 0x00);
			}
		//*flag_ptr = 0;
		flag = 0;
	}
}

void can_transmit(can_msg msg){
	mcp2515_write(MCP_TXB0SIDH, msg.idH);
	mcp2515_write(MCP_TXB0SIDL, msg.idL);
	mcp2515_write(MCP_TXB0DLC, msg.length);
	mcp2515_write(MCP_TXB0Dm, msg.data);
	mcp2515_request_to_send();
	//printf("transmitting\r\n");
}

ISR(INT0_vect){
	//*flag_ptr = 1;
	flag = 1;	
}




