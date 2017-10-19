/*
 * uart.c
 *
 *  Created on: Oct 15, 2017
 *      Author: t400
 */
#include "uart.h"

void UARTInit()  // UART beallitasa
{

	UBRR0L = UBRR_VAL; // UBRR_ERTEK also 8 bitjenek betoltese az UBRRL regiszterbe
	UBRR0H = (UBRR_VAL >> 8); // UBRR_ERTEK felso 8 bitjenek betoltese az UBRRH regiszterbe
	// UCSR0A |= (1<<U2X0);

	//Ado es Vevo aramkorok bekapcsolasa

	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}
uint8_t  UARTReceive()
{
	while (!(UCSR0A & (1 << RXC0))){} // Varakozas amig nincs uj bejovo adat
	return UDR0;
}

void UARTSendByte(uint8_t data) // Ez a fuggveny a kuldendo adatot beirja az UDR regiszter kimeno pufferjebe
{
	while (!(UCSR0A & (1 << UDRE0))){} // Varakozas amig az Ado kesz nem lesz az adatkuldesre
	UDR0 = data;
}

void UARTSendString(char * str)
{
	while (*str)
	{
		UARTSendByte(*str);
		str++;
	}
}
char UARTSend_uint8(uint8_t number)
{
	uint8_t data;
	if(number > 99){
		data = number / 100;
		number -= data * 100;
		UARTSendByte(data + 48);
		data = number / 10;
		number -= data * 10;
		UARTSendByte(data + 48);
		UARTSendByte(number + 48);
	}else if(number > 9){
		data = number / 10;
		number -= data * 10;
		UARTSendByte(data + 48);
		UARTSendByte(number + 48);
	} else if(number >= 0){
		UARTSendByte(number + 48);
	}
}

