/*
 * spi.c
 *
 * Created: 04.10.2021 14:31:55
 *  Author: kathersi
 */ 

//SPI communication driver

#include "spi.h"
#include <stdint.h>
#include <avr/io.h>


//Needed for initializing the SPI as a master
void SPI_MasterInit(void){
	// Set MOSI and SCK output, all others input, set CS as output
	DDRB |= (1<<PB5)|(1<<PB7)|(1<<PB4);
	DDRB &= ~(1<<PB6);
	
	PORTB &= ~(1<<PB4);
	
	// Enable SPI, Master, set clock rate fck/16 & select mode 00
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);

	SPCR &= ~(1<<CPOL);
	SPCR &= ~(1<<CPHA);
}

void SPI_MasterTransmit(char cData){
	// Start transmission 
	SPDR = cData;
	// Wait for transmission complete 
	while(!(SPSR & (1<<SPIF)));
}


char SPI_MasterReceive(){
	SPI_MasterTransmit(' ');
	return SPDR;
}  
