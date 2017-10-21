#ifndef INC_ADC_H
#define INC_ADC_H
    #include "main.h"
    #define ADC0    0
    #define ADC1    1
    #define ADC2    2
    #define ADC3    3
    #define ADC4    4
    #define ADC5    5
    #define ADC6    6
    #define ADC7    7
    #define ADC_1v1 14
    #define ADC_GND 15


    void adc_init();
    uint8_t adc_read(uint8_t channel);
    void adc_startconversion(uint8_t channel);
#endif