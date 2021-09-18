/*
 * GccApplication1.c
 *
 * Created: 06.09.2021 13:09:12
 * Author : amnaw
 */ 
#define F_CPU 4915200 // Clock Speed

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void USART_Init( unsigned int ubrr );
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );

int main(void)
{
	//DDRA |= (1 << PA0);
	////PORTA = 0x1;
	//while (1) {
		//
		//PORTA |= (1 << PA0);
		//_delay_ms(1000);
		//PORTA &= ~(1 << PA0);
		//_delay_ms(1000);
		////
	//}
	
// 	double del = 250;
// 	int i = 1;
// 	DDRA |= (1 << PA0);
// 	while(i > 0){
// 		if(i %2 == 0){
// 			PORTA &= ~(1<<PA0);
// 		}
// 		else{
// 			PORTA |= (1<<PA0);
// 		}
// 		_delay_ms(del);
// 		i++;
// 	}
	
	
	USART_Init ( MYUBRR );
	
	fdevopen(USART_Transmit, USART_Receive);
	
	
	while (1) {
		char c = USART_Receive();
	
		printf("Character received: %c\r\n", c);
	}
}

void USART_Init( unsigned int ubrr )
{
	/* Set baud rate */
	UBRR1H = (unsigned char)(ubrr>>8);
	UBRR1L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	/* Set frame format: 8data, 2stop bit */
	UCSR1C = (1<<URSEL1)|(1<<USBS1)|(3<<UCSZ10);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR1A & (1<<UDRE1)) );
	/* Put data into buffer, sends the data */
	UDR1 = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR1A & (1<<RXC1)) );
	/* Get and return received data from buffer */
	return UDR1;
}
