/*
 * main.c
 *
 *  Created on: Oct 15, 2017
 *      Author: t400
 */

#include "main.h"

struct payload payload;
uint8_t *adcdata_dest_array[] = {&payload.direction, &payload.speed, &payload.frontlight};
uint8_t adcdata_source_array[] = {VR1_DIRECTION_Y_ADC, VR3_SPEEDCONTROL_ADC, VR4_LIGTHCONTROL_ADC};
uint8_t adcdata_index = 0;
volatile uint8_t packet_sent = 0;

int main()
{

    // Prescaller 1024, CTC mode, interrupt on every ~1000ms
    OCR1A = 7812;
    TCCR1B |= BIT(CS12) | BIT(CS10) | BIT(WGM12);
    TIMSK1 = BIT(OCIE1A);

    //ADC init in ISR mode
    ADMUX |= BIT(REFS0) | BIT(ADLAR);
    ADCSRA |= BIT(ADEN) | BIT(ADPS2) | BIT(ADPS1) | BIT(ADPS0) | BIT(ADIE);
    DIDR0 = 0xFF;
    adc_startconversion(adcdata_source_array[0]); //start rolling, ISR will take controll
    UARTInit();
    adc_init();
    nRF2401_init(NRF_CSN_1, NRF_CE_1);
    nRF2401_reset_IRQ(NRF_CSN_1);
    UARTSendString("  NRF_Config : ");
    UARTSend_uint8(nRF_2401_reg_read(CONFIG, NRF_CSN_1));
    UARTSendString("  NRF_Status : ");
    UARTSend_uint8(nRF_2401_reg_read(STATUS, NRF_CSN_1));

    sei();

    while (1)
    {
        if (packet_sent)
        {
            UARTSendString("Radio status : ");
            UARTSend_uint8(nRF_2401_reg_read(STATUS, NRF_CSN_1));
            UARTSendString("  VR2_x : ");
            UARTSend_uint8(payload.direction);
            UARTSendString("  VR3 : ");
            UARTSend_uint8(payload.speed);
            UARTSendString("  VR4 : ");
            UARTSend_uint8(payload.frontlight);
            UARTSendString("\n\r");
            packet_sent = 0;
        }
    }

    return 0;
}
ISR(TIMER1_COMPA_vect)
{
    if (adcdata_index == 3)
    {
        // UARTSendString("\n\rTIM\n\r");
        payload.header_status = (HEADER_CONTROLLING << HEADER_OFFSET);
        payload.header_status |= switch_values();
        adcdata_index = 0;
        adc_startconversion(adcdata_source_array[0]);
        nRF2401_transmit_payload(&payload, NRF_CSN_1, NRF_CE_1);
        nRF2401_reset_IRQ(NRF_CSN_1);
        
        packet_sent = 1;
    }
    
}

ISR(ADC_vect)
{
    // UARTSendString("\n\rADC\n\r");
    //store the adc result in a designed variable
    *adcdata_dest_array[adcdata_index] = ADCH;
    //If we reach 4 as index we are done, the timer interupt will restarting the conversion
    if (adcdata_index < 3)
    {
        adcdata_index++;
        adc_startconversion(adcdata_source_array[adcdata_index]);
    }
}
void init_switch()
{
    // PD3-PD7 input
    SW_DDR &= ~(0xf8);
}
uint8_t switch_values()
{
    uint8_t pinvalue = (PIND >> 3);
    return pinvalue;
}