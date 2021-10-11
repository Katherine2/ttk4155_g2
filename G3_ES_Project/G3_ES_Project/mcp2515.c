/*
 * mcp2515.c
 *
 * Created: 04.10.2021 14:36:52
 *  Author: kathersi
 */ 

//Low level driver for setting up the CAN controller and accessing its control and status registers

#include "mcp2515.h"
#include "spi.h"
#include <avr/io.h>

int status;

uint8_t mcp2515_init(){
	SPI_MasterInit();
	uint8_t value;
	
	mcp2515_reset();
	/*
	value = mcp2515_read(MCP_CANSTAT);
	if((value & MODE_MASK) != MODE_CONFIG){
		printf("MCP2515 is NOT in configuration mode after reset!\n");
		return 1;
	}
	//More initialization
	*/
	return 0;
}

uint8_t mcp2515_read(uint8_t address){
	uint8_t result;
	
	PORTB &= ~(1<<PB4);
	
	SPI_MasterTransmit(MCP_READ);
	SPI_MasterTransmit(address);
	result = SPI_MasterReceive();
	
	PORTB |= (1<<PB4);
	
	return result;
}

void mcp2515_write(uint8_t address, uint8_t data){
	
	uint8_t result;
	
	PORTB &= ~(1<<PB4);
	
	SPI_MasterTransmit(MCP_WRITE);
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(data);
	
	PORTB |= (1<<PB4);
}

void mcp2515_reset(){
	//Needs to put MCP2515 in configuration mode
	mcp2515_write(MCP_CANCTRL, MODE_CONFIG);
}


//REALLY NOT SURE
void mcp2515_request_to_send(){
	mcp2515_write(MCP_TXB0CTRL, 0x07);
}

//WHAT ADDRESS DO WE NEED TO USE FOR THE MCP_WRITE FUNCTION??
uint8_t mcp2515_read_status(){
	SPI_MasterTransmit(MCP_READ_STATUS);
	return SPI_MasterReceive();
	//mcp2515_write(MCP_CANINTF, MCP_READ_STATUS);
	//mcp2515_read(MCP_CANINTF);
}

void mcp2515_bit_modify(uint8_t address, uint8_t data_mask, uint8_t data){
	mcp2515_write(address, (data_mask & data));
}


//NEED TO RETURN SOMETHING!!
void mcp2515_read_array(uint8_t address, uint8_t length){
	//char result[];
	for (int i=0 ; i<length; i++){
		//result[i] = mcp2515_read(address+i);
		printf(mcp2515_read(address+i));
	}
//return result;
}