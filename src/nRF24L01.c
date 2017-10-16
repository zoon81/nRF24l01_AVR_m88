/*
 * nRF24L01.c
 *
 *  Created on: Oct 15, 2017
 *      Author: t400
 */
#include "main.h"

void nRF2401_init() {
	SETBIT(DDRB, DD_CE);
	spi_init();
	SETBIT(PORTB, CSN); // set ChipSelect to high this is deselect the device
}

uint8_t nRF_2401_transfer(uint8_t data) {
	CLEARBIT(PORTB, CSN); //Activate Chip Selection on nRF2401
	_delay_us(10);
	uint8_t result = spi_transfer(data);
	SETBIT(PORTB, CSN); //Deactivate Chip Selection on nRF2401
	return result;
}

void nRF_2401_reg_write_m(uint8_t reg, uint8_t *value, uint8_t size) {
	CLEARBIT(PORTB, CSN); //Activate Chip Selection on nRF2401
	_delay_us(10);
	spi_transfer(W_REGISTER + reg);
	uint8_t index;
	for (index = 0; index < size; index++) {
		spi_transfer(value[index]);
		_delay_us(10);
	}
	SETBIT(PORTB, CSN); //deactivate ChipSelect on nRF2401
}

void nRF_2401_reg_write_s(uint8_t reg, uint8_t data) {
	CLEARBIT(PORTB, CSN); //Activate Chip Selection on nRF2401
	_delay_us(10);
	spi_transfer(W_REGISTER + reg);
	spi_transfer(data);
	SETBIT(PORTB, CSN); //Deactivate Chip Selection on nRF2401
}

uint8_t nRF_2401_reg_read(uint8_t reg) {
	CLEARBIT(PORTB, CSN); //Activate Chip Selection on nRF2401
	_delay_us(10);
	spi_transfer(R_REGISTER + reg);
	uint8_t result = spi_transfer(NOP);
	SETBIT(PORTB, CSN); //Deactivate Chip Selection on nRF2401
	return result;
}
