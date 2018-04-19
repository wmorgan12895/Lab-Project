/*
 * ping.c
 */

#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "timer.h"

volatile enum{LOW, HIGH, DONE} state;
volatile unsigned rising_time;// start time of the return pulse
volatile unsigned falling_time;// end time of the return pulse

volatile unsigned long last_time = 0;
volatile unsigned long current_time = 0;
volatile unsigned long time_diff = 0;
volatile int updateFlag = 0;

unsigned long delta(void){
    return time_diff / 16000;
}
void gpio_init(void){
    //enable clock for GPIO PORTB
    SYSCTL_RCGCGPIO_R |= BIT1;

    //Enabling PORTB pin 3
    GPIO_PORTB_DEN_R |= BIT3;
}

void TIMER3B_Handler(void){
    TIMER3_ICR_R |= TIMER_ICR_CBECINT;
    last_time = current_time;
    current_time = TIMER3_TBR_R;

    if (updateFlag == 1){
        if (current_time < last_time){ //accounting for overflow here by setting timer to 0
            TIMER3_TBR_R = 0;
            current_time = 0;
            last_time = 0;
        }else{
            time_diff = current_time - last_time;
        }
        updateFlag = 0;
    }else if (updateFlag == 0){
        updateFlag = 1;
    }
}

void start_timer(void){
    //enable the clock on the timer3
    SYSCTL_RCGCTIMER_R |= BIT3;

    TIMER3_CTL_R &= ~TIMER_CTL_TBEN;

    //Sets the TIMER to 16 bit mode
    TIMER3_CFG_R |= 0x4;

    //Turn on TIMERB and tells it to look for both positive and negative edges
    TIMER3_CTL_R |= 0xC00;
    //Set TIMER to Capture Mode, Edge-Time Mode, and Count Up
    TIMER3_TBMR_R |= 0x17;

    //clears TIMER3 time-out interrupt flags
    TIMER3_ICR_R = (TIMER_ICR_TBTOCINT | TIMER_ICR_CBECINT); //| TIMER_ICR_TBMCINT | TIMER_ICR_CBMCINT);

    //Enable the capture interrupt.
    TIMER3_IMR_R |= (TIMER_IMR_TBTOIM | TIMER_IMR_CBEIM);

    //Enable the interrupt register.  Since we are using TIMER3B we need interupt 36 which is bit 5 on NVIC_EN3
    NVIC_EN1_R = 0x10;

    //register TIMER3 interrupt handler
    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    //initialize global interrupts
    IntMasterEnable();

    TIMER3_CTL_R |= TIMER_CTL_TBEN;
}

void ping_init(void){
    gpio_init();
    start_timer();
}

float distance(void){
    float distance = 0;
    distance = time_diff * 34000.0 / 2.0 / 16000000.0;
    return distance;
}

uint32_t pulse(void){
    GPIO_PORTB_AFSEL_R &= 0xF7;

     GPIO_PORTB_DIR_R |= 0x08;//0b1000 //this one
     // set PB3 as output
     GPIO_PORTB_DATA_R |= 0x08;
     // set PB3 to high
     timer_waitMicros(10); //waiting 10 just in case
     // wait at least 5 microseconds based on data sheet
     GPIO_PORTB_DATA_R &= 0xF7;
      // set PB3 to low
     GPIO_PORTB_DIR_R &= 0xF7;
     // set PB3 as input

     GPIO_PORTB_AFSEL_R |= 0x08;
     GPIO_PORTB_PCTL_R |= 0x7000;
}

float get_sonar_distance(void){
    pulse();

    return distance();

}
