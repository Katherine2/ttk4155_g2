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


void can_init(void){
	mcp2515_init();
	mcp2515_write(MCP_CANINTE, 0X03);		//enables the reception complete flag (for the interrupt)
	mcp2515_write(MCP_CANCTRL, MODE_LOOPBACK);
 	uint8_t value;

	value = mcp2515_read(MCP_CANSTAT);

	if((value & MODE_MASK) != MODE_LOOPBACK){
		printf("MCP2515 is NOT in loopback mode after reset!\n");
	}			

	// Disable global interrupts
	cli();
	// Interrupt on falling edge
	MCUCR |= (1<<ISC01);
	// Enable interrupt 
	GICR |= (1<<INT0);
	// Enable global interrupts
	sei();
	}

void can_receive(void){
	can_msg msg;
	if(mcp2515_read_status() & 0x01){
		msg.id = (mcp2515_read(MCP_RXB0SIDH) << 3)|(mcp2515_read(MCP_RXB0SIDL) >> 5);
		msg.length = mcp2515_read(MCP_RXB0DLC);
		for(uint8_t i = 0; i < (msg.length); i++){
			msg.data[i] = mcp2515_read(MCP_RXB0D0 + i);
		}
		mcp2515_bit_modify(MCP_CANINTF,0x01, 0x00);
	}
		
	if(mcp2515_read_status() & 0x02){
		msg.id = (mcp2515_read(MCP_RXB1SIDH) << 3)|(mcp2515_read(MCP_RXB1SIDL) >> 5);
		msg.length = mcp2515_read(MCP_RXB1DLC);
		for(uint8_t i = 0; i < (msg.length); i++){
			msg.data[i] = mcp2515_read(MCP_RXB1D0 + i);
		}
		mcp2515_bit_modify(MCP_CANINTF,0x02, 0x00);
	}
			
	printf("data: %c\r\nlength: %d\r\nid: %d\r\n\n", msg.data[0], msg.length, msg.id);
}

void can_transmit(can_msg msg){
	mcp2515_write(MCP_TXB0SIDH, msg.id >> 3);
	mcp2515_write(MCP_TXB0SIDL, msg.id << 5);
	mcp2515_write(MCP_TXB0DLC, msg.length);
	for(uint8_t i = 0; i < (msg.length); i++){
		mcp2515_write(MCP_TXB0D0 + i, msg.data[i]);
	}
	mcp2515_request_to_send(0);
}

ISR(INT0_vect){
	can_receive();
}


