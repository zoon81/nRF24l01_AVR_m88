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

#define BIT(x) (1 << (x))
#define SETBITS(x, y) ((x) |= (y))
#define SETBIT(x, y) SETBITS((x), (BIT((y))))

#define CLEARBITS(x, y) ((x) &= (~(y)))
#define CLEARBIT(x, y) CLEARBITS((x), (BIT((y))))

#define NRF_CE_1 PB1
#define NRF_CSN_1 PB2

#define NRF_CE_2 PB7
#define NRF_CSN_2 PB6

#define payload_len 5

#define VR2_X_ADC ADC0
#define VR1_DIRECTION_Y_ADC ADC1
#define VR4_LIGTHCONTROL_ADC ADC2
#define VR3_SPEEDCONTROL_ADC ADC3

#define HEADER_OFFSET       6
#define HEADER_PING         0
#define HEADER_CONTROLLING  1
#define HEADER_GETSTATUS    2
//header_status variable holds header status and button status. first 2 bit,next 2 bit sw2 state 00 for A, 01 for B, 11 for C. next bit is P02 button, next bit is P05 button 
// Like H H X X A B C P P (bit5 and bit4 not defined yet)
struct payload
{
    uint8_t header_status;
    uint8_t frontlight;
    uint8_t speed;
    uint8_t direction;
};
uint8_t switch_values();

#endif /* INC_MAIN_H_ */
