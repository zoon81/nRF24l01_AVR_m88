/*
 * uart.h
 *
 *  Created on: Oct 15, 2017
 *      Author: t400
 */

#ifndef UART_H_
#define UART_H_

#include "main.h"

#define USART_BAUDRATE 9600
#define UBRR_VAL ((F_CPU / (USART_BAUDRATE * 16UL)) - 1) // UBRR

void UARTInit();
uint8_t  UARTReceive();
void UARTSendByte(uint8_t data);
void UARTSendString(char * str);
void UARTSend_uint8(uint8_t number);

#endif /* UART_H_ */
