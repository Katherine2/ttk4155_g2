/*
 * spi.h
 *
 * Created: 04.10.2021 14:32:05
 *  Author: kathersi
 */ 


#ifndef SPI_H_
#define SPI_H_



void SPI_MasterInit(void);																			//Where is the function for this?
void SPI_MasterTransmit(char cData);
char SPI_MasterReceive();


#endif /* SPI_H_ */