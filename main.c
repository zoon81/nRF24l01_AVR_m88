/*
 * main.c
 *
 *  Created on: Oct 15, 2017
 *      Author: t400
 */

#include "main.h"

int main(){
    nRF2401_init(NRF_CSN_1, NRF_CE_1 );
    nRF2401_init(NRF_CSN_2, NRF_CE_2 );
    nRF_2401_reg_read(CONFIG, NRF_CSN_1);
    nRF_2401_reg_read(STATUS, NRF_CSN_1);
    nRF_2401_reg_read(CONFIG, NRF_CSN_2);
    nRF_2401_reg_read(STATUS, NRF_CSN_2);
    uint8_t data[5];
    uint8_t i;
    for(i=0; i<5 ;i++){
        data[i]= (i+1);
    }
    nRF2401_reset_IRQ(NRF_CSN_2);
    nRF2401_set_receiver_mode(NRF_CSN_2, NRF_CE_2);

    nRF2401_reset_IRQ(NRF_CSN_1);
    nRF2401_transmit_payload(data, NRF_CSN_1, NRF_CE_1);
    nRF_2401_reg_read(CONFIG, NRF_CSN_1);
    nRF_2401_reg_read(STATUS, NRF_CSN_1);

    nRF2401_receive_payload(NRF_CSN_2, NRF_CE_2, data);

    adc_init();
    UARTInit();

    while(1){
        UARTSend_uint8(adc_read(0));
        UARTSendString("\n\r");
        _delay_ms(500);
    }
    
    return 0;
}
