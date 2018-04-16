/*
 * movement.h
 *
 *  Created on: Jan 23, 2018
 *      Author: wrmorgan
 */
#include "lcd.h"
#include "open_interface.h"

#ifndef MOVEMENT_H
#define  MOVEMENT_H

void move_forward_with_bumps(oi_t *sensor, int centimeters);

void move_forward(oi_t *sensor, int centimeters);

void move_backwards(oi_t *sensor, int centimeters);

void turn_clockwise(oi_t *sensor, int degrees);

void turn_counterclockwise(oi_t *sensor, int degrees);


#endif
