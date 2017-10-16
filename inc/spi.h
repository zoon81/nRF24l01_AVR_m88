/*
 * spi.h
 *
 *  Created on: Oct 15, 2017
 *      Author: t400
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_
#include "main.h"

#define DD_MOSI		DDB3
#define DD_SCK		DDB5
#define DD_CSN		DDB2
#define CSN			PB2

uint8_t spi_transfer(uint8_t data );

#endif /* INC_SPI_H_ */
