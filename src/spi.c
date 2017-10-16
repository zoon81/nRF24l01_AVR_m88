/*
 * spi.c
 *
 *  Created on: Oct 15, 2017
 *      Author: t400
 */

#include "spi.h"

void spi_init(){
	DDRB |= (1 << DD_SCK) | (1 << DD_MOSI) | (1 << DD_CSN);
	SPCR |= (1 << SPR1) | ( 1 << SPR0 ) | (1 << MSTR) | (1 << SPIE);
}

uint8_t spi_transfer(uint8_t data ){
	while(SPSR & BIT(SPIF) ); //wait until the previous transmission complete
	SPDR = data;
	while(SPSR & BIT(SPIF) ); // wait until this transmission is complete
	return SPDR;
}
