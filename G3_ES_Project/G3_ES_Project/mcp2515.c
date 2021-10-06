/*
 * mcp2515.c
 *
 * Created: 04.10.2021 14:36:52
 *  Author: kathersi
 */ 

//Low level driver for setting up the CAN controller and accessing its control and status registers

#include "mcp2515.h"
#include "spi.h"

uint8_t mcp2515_init(){
	uint8_t value;
	
	SPI_MasterInit();
	mcp2515_reset();
	
	mcp2515_read(MCP_CANSTAT, &value);
	if((value & MODE_MASK) != MODE_CONFIG){
		printf("MCP2515 is NOT in configuration mode after reset!\n");
		return 1;
	}
	//More initialization
	
	return 0;
}

uint8_t mcp2515_read(uint8_t address){
	uint8_t result;
	
	PORTB &= ~(1>>PB0);
	
	SPI_MasterTransmit(MCP_READ);
	SPI_MasterTransmit(address);
	result = SPI_READ();
	
	PORTB |= (1<<PB0);
	
	return result;
}

void mcp2515_reset(){
	//Needs to put MCP2515 in configuration mode
}

ISR(INT0_vect){
	MCP_CANINTE = 0x01;
	int status = MCP_CANINTF & 0x01;
}
