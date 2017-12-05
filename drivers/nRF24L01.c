/*
 * nRF24L01.c
 *
 *  Created on: Oct 15, 2017
 *      Author: t400
 */
#include "main.h"
// init nRF24l01+ with following setting and setup CE pin
void nRF2401_init(uint8_t NRF_CSN, uint8_t NRF_CE) {
	SETBIT(DDRB, NRF_CE);
	spi_init(NRF_CSN);
	nRF_2401_reg_write_s(EN_AA, 0x01, NRF_CSN);			//Enable auto acknowledgement data pipe 0
	nRF_2401_reg_write_s(SETUP_RETR, 0x15, NRF_CSN);	////Auto Retransmit Delay: 500μS and  Up to 5 Re-Transmit on fail of AA
	nRF_2401_reg_write_s(EN_RXADDR, 0x01, NRF_CSN);		//Enable data pipe 0.
	nRF_2401_reg_write_s(SETUP_AW, 0x03, NRF_CSN);		//5bytes Address Width
	nRF_2401_reg_write_s(RF_CH, 0x01, NRF_CSN);			//RF Chanel 2.401 GHz
	nRF_2401_reg_write_s(RF_SETUP, 0x02, NRF_CSN);		//250kbps, RF output power 0dBm (max)

	uint8_t device_addr[5] = {0x12,0x12,0x12,0x12,0x12};
	nRF_2401_reg_write_m(RX_ADDR_P0, device_addr, 5, NRF_CSN);  
	nRF_2401_reg_write_m(TX_ADDR, device_addr, 5, NRF_CSN);

	nRF_2401_reg_write_s(RX_PW_P0, payload_len, NRF_CSN);	//5byte data in data pipe0 in this case
	nRF_2401_reg_write_s(CONFIG, 0x7E, NRF_CSN);			//PTX, PWR_UP, CRC_2byte, Enable CRC, No_IRQ
}
//Write multiple byte register
void nRF_2401_reg_write_m(uint8_t reg, uint8_t *value, uint8_t size, uint8_t CSN) {
	CLEARBIT(PORTB, CSN); //Activate Chip Selection on nRF2401
	_delay_us(10);
	spi_transfer(W_REGISTER + reg);
	uint8_t index;
	for (index = 0; index < size; index++) {
		spi_transfer(value[index]);
		_delay_us(10);
	}
	SETBIT(PORTB, CSN); //deactivate ChipSelect on nRF2401
	_delay_us(20);
}
//Write a single byte register
void nRF_2401_reg_write_s(uint8_t reg, uint8_t data, uint8_t CSN) {
	CLEARBIT(PORTB, CSN); //Activate Chip Selection on nRF2401
	_delay_us(20);
	spi_transfer(W_REGISTER + reg);
	spi_transfer(data);
	SETBIT(PORTB, CSN); //Deactivate Chip Selection on nRF2401
	_delay_us(20);
}
//Read a single byte register
uint8_t nRF_2401_reg_read(uint8_t reg, uint8_t CSN) {
	CLEARBIT(PORTB, CSN); //Activate Chip Selection on nRF2401
	_delay_us(20);
	spi_transfer(R_REGISTER + reg);
	uint8_t result = spi_transfer(NOP);
	SETBIT(PORTB, CSN); //Deactivate Chip Selection on nRF2401
	_delay_us(20);
	return result;
}
//Sending a payload and transmit it
void nRF2401_transmit_payload(struct payload *payload, uint8_t CSN, uint8_t NRF_CE){
	CLEARBIT(PORTB, CSN); //Activate Chip Selection on nRF2401
	//_delay_us(10);
	spi_transfer(FLUSH_TX);
	SETBIT(PORTB, CSN);
	//_delay_us(10);

	CLEARBIT(PORTB, CSN); //Activate Chip Selection on nRF2401
	spi_transfer(W_TX_PAYLOAD);
	spi_transfer(payload->header_status);
	spi_transfer(payload->direction);
	spi_transfer(payload->speed);
	spi_transfer(payload->frontlight);
	spi_transfer(NOP);							// this is for the payload width,
	//If you not putting exactly 5 byte intro TX_FIFO you get fucked,
	//you get CRC missmatch on RX side and you never get ACK from receiver. This gave me 10h troubleshotting
	SETBIT(PORTB, CSN);							
	//_delay_ms(10);
	SETBIT(PORTB, NRF_CE);					//Activating trasnmitter
	_delay_us(20);
	CLEARBIT(PORTB, NRF_CE);
	_delay_ms(5);						//This is for the auto retransmission if no ack received
}
//Reseting Interupt bits
void nRF2401_reset_IRQ(uint8_t CSN){
	nRF_2401_reg_write_s(STATUS, 0x70, CSN);
}
//Set the nRF24l01+ to receive mode
void nRF2401_set_receiver_mode(uint8_t CE){
	SETBIT(PORTB, CE);									// Activate receiver
}
//Read the received payload. This function unset the receiving mode too
void nRF2401_receive_payload(uint8_t CSN, uint8_t CE, struct payload *buffer){
	while( !(nRF_2401_reg_read(STATUS, CSN) & BIT(RX_DR)) ){  	//wait until payload received and processed
		_delay_us(10);
	}
	CLEARBIT(PORTB, CE);										//Disable receive mode
	CLEARBIT(PORTB, CSN);
	spi_transfer(R_RX_PAYLOAD);
	
	buffer->header_status = spi_transfer(NOP);
	buffer->frontlight = spi_transfer(NOP);
	buffer->speed = spi_transfer(NOP);
	buffer->direction = spi_transfer(NOP);
	
	SETBIT(PORTB, CSN);
}
//Interupt mode version of the above function, non blocking payload read
void nRF2401_receive_payload_it(uint8_t CSN, uint8_t CE, struct payload *buffer){
	CLEARBIT(PORTB, CE);										//Disable receive mode, set power donw mode
	CLEARBIT(PORTB, CSN);
	spi_transfer(R_RX_PAYLOAD);
	
	buffer->header_status = spi_transfer(NOP);
	buffer->frontlight = spi_transfer(NOP);
	buffer->speed = spi_transfer(NOP);
	buffer->direction = spi_transfer(NOP);
	
	SETBIT(PORTB, CSN);

	//Flush RX reg
	CLEARBIT(PORTB, CSN); //Activate Chip Selection on nRF2401
	_delay_us(10);
	spi_transfer(FLUSH_RX);
	SETBIT(PORTB, CSN);
	_delay_us(10);

	//Clear all interupt in status register
	nRF_2401_reg_write_s(STATUS, 0x70, NRF_CSN_2);
}
