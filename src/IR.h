/*
 * IR.h
 *
 *  Created on: Mar 1, 2016
 *      Author: mhoppe
 */

#ifndef IR_H_
#define IR_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "timer.h"

/// Initialize PORTB0:6 to Communicate with IR
void ir_init(void);
//int get_ir_distance(void);
//int estimate_distance(int binary_value);

#endif /* IR_H_ */
