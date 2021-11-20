/*
 * uart.c
 *
 * Created: 19.09.2021 12:14:13
 *  Author: kathersi
 */ 

#include "uart.h"

void USART_Init( unsigned int ubrr )
{
	// Set baud rate
	UBRR1H = (unsigned char)(ubrr>>8);
	UBRR1L = (unsigned char)ubrr;
	// Enable receiver and transmitter
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	// Set frame format: 2 stop bits,  8 data bits
	UCSR1C = (1<<URSEL1)|(1<<USBS1)|(3<<UCSZ10);		//URSEL1 sets it to synchronous communication
}

void USART_Transmit( unsigned char data )
{
	// Wait for empty transmit buffer
	while ( !( UCSR1A & (1<<UDRE1)) );					//UDRE is a data empty flag
	// Put data into buffer, sends the data 
	UDR1 = data;
}

unsigned char USART_Receive( void )
{
	// Wait for data to be received 
	while ( !(UCSR1A & (1<<RXC1)) );					//RXC1 is a receive complete bit
	// Get and return received data from buffer 
	return UDR1;
}