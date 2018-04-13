/*
 * obj_dect.h
 */

#ifndef OBJ_DECT_H_
#define OBJ_DECT_H_

#include <stdio.h>
#include <inc/tm4c123gh6pm.h>

void obj_init(int ir_input[], float sn_input[], int n);

void obj_run();

void get_rolling_average();

void get_bits();

void get_number_edges();

void get_objects();

int get_n_objects();

float get_obj_dist(int index);

int get_obj_location(int index);

int get_obj_width(int index);

#endif /* OBJ_DECT_H_ */
