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
#include <stdio.h>

int status;

void mcp2515_init(void){
	uint8_t value;

	SPI_MasterInit();
	mcp2515_reset();
	
	value = mcp2515_read(MCP_CANSTAT);		//sets it to configuration mode
	if((value & MODE_MASK) != MODE_CONFIG){
		printf("MCP2515 is NOT in configuration mode after reset!\n");
	}
}

//All of the functions below follow the steps described in section 12 of the MCP2515 datasheet

uint8_t mcp2515_read(uint8_t address){
	uint8_t result;
	
	PORTB &= ~(1<<PB4);					//pull !CS low
	
	SPI_MasterTransmit(MCP_READ);		//send read instruction
	SPI_MasterTransmit(address);		//send the address we want to read from
	result = SPI_MasterReceive();		//the data is stored
	
	PORTB |= (1<<PB4);					//pull !CS to high
	
	return result;
}

void mcp2515_write(uint8_t address, uint8_t data){
	PORTB &= ~(1<<PB4);
	SPI_MasterTransmit(MCP_WRITE);
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(data);
	PORTB |= (1<<PB4);
}

void mcp2515_reset(void){
	PORTB &= ~(1<<PB4);
	SPI_MasterTransmit(MCP_RESET);
	PORTB |= (1<<PB4);
}


void mcp2515_request_to_send(uint8_t buffer){
	PORTB &= ~(1<<PB4);
	switch(buffer){
	case 0:
		SPI_MasterTransmit(MCP_RTS_TX0);
	break;
	case 1:
		SPI_MasterTransmit(MCP_RTS_TX1);
	break;
	case 2:
		SPI_MasterTransmit(MCP_RTS_TX2);
	break;
	case 9:
		SPI_MasterTransmit(MCP_RTS_ALL);
	break;
	default:
		printf("Not a valid entry\n\r try again");
	break;
	}
	PORTB |= (1<<PB4);
}

uint8_t mcp2515_read_status(void){
	uint8_t result;
	PORTB &= ~(1<<PB4);
	SPI_MasterTransmit(MCP_READ_STATUS);
	result = SPI_MasterReceive();
	PORTB |= (1<<PB4);
	return result;
}

void mcp2515_bit_modify(uint8_t address, uint8_t data_mask, uint8_t data){
	PORTB &= ~(1<<PB4);
	SPI_MasterTransmit(MCP_BITMOD);
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(data_mask);
	SPI_MasterTransmit(data);
	PORTB |= (1<<PB4);
}
