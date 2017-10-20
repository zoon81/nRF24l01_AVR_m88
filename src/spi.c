/*
 * spi.c
 *
 *  Created on: Oct 15, 2017
 *      Author: t400
 */

#include "spi.h"
// init SPI in master mode with F_CPU / 128 prescaler, and setup ChipSelect pin
void spi_init(uint8_t CSN){
	//If SS is configured as an input, it must be held high to ensure Master SPI operation
	DDRB |= (1 << DD_SCK) | (1 << DD_MOSI) | (1 << CSN);
	SPCR |= (1 << SPR1) | ( 1 << SPR0 ) | (1 << MSTR) | (1 << SPE);
	SETBIT(PORTB, PB2);
	SETBIT(PORTB, CSN);
}
// sending a byte over SPI. ChipSelect management not included
uint8_t spi_transfer(uint8_t data){
	SPDR = data;
	while(!(SPSR & BIT(SPIF) )); // wait until this transmission is complete
	return SPDR;
}
