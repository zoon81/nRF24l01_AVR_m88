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
#include <avr/interrupt.h>

#include "spi.h"
#include "nRF24L01.h"
#include "uart.h"
#include "adc.h"
#include "payload_def.h"

#define BIT(x) (1 << (x))
#define SETBITS(x, y) ((x) |= (y))
#define SETBIT(x, y) SETBITS((x), (BIT((y))))
#define CLEARBITS(x, y) ((x) &= (~(y)))
#define CLEARBIT(x, y) CLEARBITS((x), (BIT((y))))

#define NRF_CE_1 PB1
#define NRF_CSN_1 PB2
#define NRF_CE_2 PB7
#define NRF_CSN_2 PB6

uint8_t switch_values();

#endif /* INC_MAIN_H_ */
