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

#define SS PB4
#define MOSI PB5
#define MISO PB6
#define SCK PB7

//Needed for initializing the SPI as a master
void SPI_MasterInit(void){
	// Set MOSI and SCK output, all others input, set CS as output
	DDRB |= (1<<MOSI)|(1<<SCK)|(1<<SS);
	DDRB &= ~(1<<MISO);
	
	// Enable SPI, Master, set clock rate fck/16 & select mode 00
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);

	SPCR &= ~(1<<CPOL);
	SPCR &= ~(1<<CPHA);

	PORTB |= (1<<SS);
}

void SPI_MasterTransmit(char cData){
	// Start transmission 
	SPDR = cData;
	// Wait for transmission complete 
	while(!(SPSR & (1<<SPIF)));
}

//to receive, one must transmit a dummy character
char SPI_MasterReceive(void){
	SPI_MasterTransmit('a');
	return SPDR;
}  
