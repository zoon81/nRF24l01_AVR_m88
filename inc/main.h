/*
 * main.h
 *
 *  Created on: Oct 15, 2017
 *      Author: t400
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"
#include "nRF24L01.h"
#include "uart.h"

#define BIT(x) 			(1 << (x))
#define SETBITS(x,y) 	((x) |= (y))
#define SETBIT(x,y)	SETBITS( (x), (BIT ((y)) ))

#define CLEARBITS(x,y) 	( (x) &= (~(y)) )
#define CLEARBIT(x,y)	CLEARBITS( (x), (BIT((y))))

#endif /* INC_MAIN_H_ */
