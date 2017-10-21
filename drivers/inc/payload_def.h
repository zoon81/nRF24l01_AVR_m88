#ifndef INC_PAYLOAD_DEF_H_
#define INC_PAYLOAD_DEF_H_
#include "main.h"

#define payload_len 5
// analog sticks definition
#define VR2_X_ADC               ADC0
#define VR1_DIRECTION_Y_ADC     ADC1
#define VR4_LIGTHCONTROL_ADC    ADC2
#define VR3_SPEEDCONTROL_ADC    ADC3
// package type definision
#define HEADER_OFFSET       6
#define HEADER_PING         0
#define HEADER_CONTROLLING  1
#define HEADER_GETSTATUS    2
// switch definitions
#define SW_DDR              DDRD
#define SW_PORT             PORTD
#define sw_ststrim_l        PD3
#define sw_ststrim_r        PD4
#define sw_sw2_a            PD5
#define sw_sw2_b            PD6
#define sw_sw2_c            PD7

//header_status variable holds header status and button status. first 2 bit,next 2 bit sw2 state 00 for A, 01 for B, 11 for C. next bit is P02 button, next bit is P05 button 
// Like H H X X A B C L R (bit5 and bit4 not defined yet)
struct payload
{
    uint8_t header_status;
    uint8_t frontlight;
    uint8_t speed;
    uint8_t direction;
};

#endif