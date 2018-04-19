/*
 * servo.h
 *
 *  Created on: Mar 26, 2018
 *      Author: mhoppe
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <inc/tm4c123gh6pm.h>
#include "timer.h"

void servo_init(void);
void turn_to(int degree);
int current_position(void);


#endif /* SERVO_H_ */
