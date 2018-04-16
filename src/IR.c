/**
 * ir.c: Functions for getting distance data from the IR sensor
 *  @author Mitchell Hoppe, Kenneth Lange
 *  @date 02/27/2018
 *
 *
 */




#include "IR.h"

#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80


void ir_init(void){
    //enable ADC 0 module on port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    //enable clock for ADC
    SYSCTL_RCGCADC_R |= 0x1;
    //enable port B pin 0 to work as alternate functions
    GPIO_PORTB_AFSEL_R |= 0x01;
    //set pin to input - 0
    GPIO_PORTB_DEN_R &= 0b11101111;
    //disable analog isolation for the pin
    GPIO_PORTB_AMSEL_R |= 0x01;
    //initialize the port trigger source as processor (default)
    GPIO_PORTB_ADCCTL_R = 0x00;
    //disable SS0 sample sequencer to configure it
    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0;
    //initialize the ADC trigger source as processor (default)
    ADC0_EMUX_R = ADC_EMUX_EM0_PROCESSOR;
    //set 1st sample to use the AIN10 ADC pin
    ADC0_SSMUX0_R |= 0x000A;
    //enable raw interrupt status
    ADC0_SSCTL0_R |= (ADC_SSCTL0_IE0 | ADC_SSCTL0_END0);
    //enable oversampling to average
    ADC0_SAC_R |= ADC_SAC_AVG_64X;
    //re-enable ADC0 SS0
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;
}
//
//int get_ir_distance(void){
//    //initiate SS0 conversion
//    ADC0_PSSI_R=ADC_PSSI_SS0;
//    //wait for ADC conversion to be complete
//    while((ADC0_RIS_R & ADC_RIS_INR0) == 0){
//    //wait
//    }
//    //grab result
//    int value = ADC0_SSFIFO0_R;
//
//    return estimate_distance(value);
//
//}
//
//int estimate_distance(int binary_value){
//   return (int)(108105 / pow(binary_value, 1.166));
//}

