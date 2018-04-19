/*
 * ping.h
 */

#ifndef PING_H_
#define PING_H_
#include <inc/tm4c123gh6pm.h>
#include "timer.h"

void gpio_init(void);
uint32_t pulse(void);
void start_timer(void);
void timer_handler(void);
float distance(void);
void ping_init(void);
unsigned long delta(void);
float get_sonar_distance(void);

#endif /* PING_H_ */
