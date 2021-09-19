/*
 * uart.h
 *
 * Created: 19.09.2021 12:14:13
 *  Author: kathersi
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

void USART_Init( unsigned int ubrr );

void USART_Transmit( unsigned char data );

unsigned char USART_Receive( void );