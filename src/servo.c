/*
 * servo.c
 */

#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "timer.h"

volatile unsigned int pulse_width;
volatile int current_pos;
static int timer_end = 320000;

void servo_init(void){
        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

        //Initalize GPIO
        GPIO_PORTB_AFSEL_R |= 0b100000;
        GPIO_PORTB_PCTL_R |= 0x700000;

        GPIO_PORTB_DIR_R |= 0b100000;

        GPIO_PORTB_DEN_R |= 0b100000;

        SYSCTL_RCGCTIMER_R |= 0b10;

        //Disable timer
        TIMER1_CTL_R &= ~(0x100);

        TIMER1_CFG_R |= TIMER_CFG_16_BIT;

        TIMER1_TBMR_R |= (TIMER_TBMR_TBMR_PERIOD | TIMER_TBMR_TBAMS);
        TIMER1_TBMR_R &= ~(TIMER_TBMR_TBCMR);

        TIMER1_CTL_R &= ~(TIMER_CTL_TBPWML);

        TIMER1_TBILR_R = (timer_end & 0xFFFF);

        TIMER1_TBPR_R = (timer_end >> 16);

        pulse_width = 0;
        turn_to(pulse_width);

        TIMER1_TBMATCHR_R = ((timer_end - pulse_width) & 0xFFFF);

        TIMER1_TBPMR_R = ((timer_end- pulse_width) >> 16);

        TIMER1_CTL_R |= TIMER_CTL_TBEN;
}

void turn_to(int degree){
    current_pos = degree;
    pulse_width = ((28992 * (degree/180.0)) + 874);

    TIMER1_TBMATCHR_R = ((timer_end - pulse_width) & 0xFFFF);

    TIMER1_TBPMR_R = ((timer_end - pulse_width) >> 16);

    timer_waitMillis(50);
}

volatile int current_position(void){
    return current_pos;
}
