/*
 * obj_dect.c
 */
 
#include "obj_dect.h"
#include <stdio.h>
#include <stdlib.h>

#include <inc/tm4c123gh6pm.h>
 
float* obj_dist;    //The distance of the objects from the sensor(cm)/
				    //AF: the number of starts/ends when running the algorithm
int* obj_location;	//The angle of the object w/r to the bot (0-179 degrees)
int* obj_width;	    //Width of the object in degrees (**maybe changed to cm when support is added**)
int n_objects;		//Number of objects detected
					//AF: firstStart index

int*     ir_data;   //The data stored from the ir_sensor, get modified as it runs through the algorithm.
float*   sn_data;   //The data stored from the sonar_sensor, get modified as it runs through the algorithm.
int size;           //The number of data point in the XX_data arrays. Should be the same as data points are taken together.

/**
 * init function for the object_detection class.
 *
 * Precondition: ir_input and sn_input are not null and both have a length of n
 *
 * Parameters:
 * int[] ir_input: the input data from the IR sensor
 * int[] sn_input: the input data from the PING sensor
 * int n: length of the previous arrays
 */
void obj_init(int ir_input[], float sn_input[], int n){
    //allocate memory for the XX_data arrays
	ir_data = (int*)malloc(sizeof(int) * n);
	sn_data = (float*)malloc(sizeof(float) * n);
	size = n;
	//Copy over the data from the parameters.
	int j;for(j=0; j < size; j++){
	    ir_data[j] = ir_input[j];
	    sn_data[j] = sn_input[j];
	}
}

/**
 * Runs the algorithm
 *
 * Precondition: obj_init has already been run with the correct data.
 */
void obj_run(){
    //Gets the rolling average for the data given. Smooths out the data more to midigate outliers.
    get_rolling_average();
    //Converts the data from the rolling average to bits (0/1) depending on if the data meets the 120 threshold.
    get_bits();
    //returns the number of starts and ends
    get_number_edges();
    //gets the information from each object that it sees.
    get_objects();
}

/**
 * Sets each data point to the average of its neighbors.
 * The rolling average is of length 5 when there is room.
 * However, at the edges where there is not enough space,
 * the average is taken of length 3 and 0. This is done to
 * maintain the lengths of the arrays to make the calculations
 * easier.
 */
void get_rolling_average(){
    //Create temp variable to put the rolling average into.
	int* tmp;
	tmp = (int*)malloc(sizeof(int) * size);
	int j;for(j=0; j < size; j++){
		int Avgir;
		        //No average, because it is at an edge
				if(j == 0 || j == size - 1)Avgir = ir_data[j];
				//Average of 3 inputs of the IR Reads
		else 	if(j == 1 || j == size - 2)Avgir = (ir_data[j-1]+ir_data[j]+ir_data[j+1])/3;
				//Average of 5 inputs of the IR Reads
		else Avgir = (ir_data[j-2]+ir_data[j-1]+ir_data[j]+ir_data[j+1]+ir_data[j+2])/5;
		tmp[j] = Avgir;
    }
	//Copy the tmp array into the ir_data array now that the rolling_average has been calculated.
	for(j=0; j < size; j++){
		ir_data[j] = tmp[j];
	}
	//Unallocate the memory of tmp
	free(tmp);
}

/**
 * Converts the rolling average data into bits(0/1)
 * depending on if the data fits the 120 threshold. This is done to get ride of background data.
 * Typically any object the is in the view of the robot will fulfill this requirement.
 * Code is not commented. Pretty self-explanatory.
 */
void get_bits(){
	int j;for(j=0; j < size; j++){
		ir_data[j] = (ir_data[j]>120 ? 1 : 0);
    }
}

/**
 * Stores the number of start and stop edges in obj_dist[0] and obj_dist[1] respectively.
 * obj_dist is used because the # of starts and stops needs to be stored in a global variable for get_objects()
 * to use. And I didn't want to create a global array for the sole purpose of storing the # of starts and stops,
 * only to be used once.
 * A start is defined as where at index k ir_data[k] is 1 and ir_data[k + 1] is 0.
 * A stop is the opposite. ir_data[k] is 0 and ir_data[k + 1] is 1.
 * This function also stores the location of the first start in n_objects. n_objects for the same reason mentioned above.
 * We want to store the location of the first start because if the edges of the data are 0, in-other-words, there are objects
 * on the edges of the arrays but we dont know much about them, we want to ignore these because we want to be able to line up
 * smoothly the starts and stops to determine where the objects are. This allows us to determine that consecutive starts and stops
 * are where the objects are.
 */
void get_number_edges(){
	obj_dist = (float*)malloc(sizeof(float) * size);//[0] = #starts, [1] = #ends
	int j;for(j=0; j < size; j++){
	    //If there is a start
		if(ir_data[j] < ir_data[j-1]){
		    //Count it.
			obj_dist[0]++;
			//update the first start variable.
			if (n_objects>j) n_objects = j;
		}
		//If a stop is found and it has a corresponding start.
		if((ir_data[j] > ir_data[j-1]) && (obj_dist[1] < obj_dist[0])){
		    //Count it.
			obj_dist[1]++;
		}
	}
	//Correct any imbalance in the number of starts and stops.
	if(obj_dist[0] > obj_dist[1]) obj_dist[0]--;
}
/**
 * Gets the location of the starts and stops that correspond to their respective objects.
 * Then gets the info from each object.
 *
 * obj_location: the location of the object in degrees w/r to the robot.
 * obj_dist: the distance from the robot to the surface of the object in cm.
 * obj_width: Width of the object in degrees.
 */
void get_objects(){
    int* starts_index;
    int* ends_index;
    //Instantiate arrays with the length of the number of objects detected
    n_objects = (int)obj_dist[1];
	starts_index 	= (int*)    malloc(sizeof   (int)   * n_objects);
    ends_index 	    = (int*)    malloc(sizeof   (int)   * n_objects);

    obj_dist        = (float*)  malloc(sizeof (float)   * n_objects);
    obj_location    = (int*)    malloc(sizeof   (int)   * n_objects);
    obj_width       = (int*)    malloc(sizeof   (int)   * n_objects);
    //Keep tracc of the index of the first empty element as we fill it up.
    int starts_index_size = 0;
    int ends_index_size = 0;

    //When a start or stop is detected. Mark its location.
	int i;for(i = n_objects; i < size;i++){
		if(ir_data[i] < ir_data[i-1]){
			starts_index[starts_index_size] = i;
			starts_index_size++;
		}
		if(ir_data[i] > ir_data[i-1]){
			ends_index[ends_index_size] = i;
			ends_index_size++;
		}
    }
	//Calculate the info for each of the objects.
	for(i = 0; i < n_objects;i++){
		obj_location[i] = (ends_index[i] + starts_index[i]) / 2;
		obj_dist[i] = sn_data[obj_location[i]];
		obj_width[i] = ends_index[i] - starts_index[i];
	}
	//Free the temporary variables.
	free(starts_index);
	free(ends_index);
}

/**
 * Returns the number of objects that the robot detected.
 *  Return: n_objects, -1 if the index is OutOfBounds
 */
int get_n_objects(){
    return n_objects;
}

/**
 * Returns the i'th object's distance from the robot
 *  Return: obj_dist[index], -1 if the index is OutOfBounds
 */
float get_obj_dist(int index){
    if(index < 0 || index >= n_objects) return -1;
    return obj_dist[index];
}

/**
 * Returns the i'th object's location in degrees
 *  Return: obj_location[index], -1 if the index is OutOfBounds
 */
int get_obj_location(int index){
    if(index < 0 || index >= n_objects) return -1;
    return obj_location[index];
}

/**
 * Returns the i'th object's width in degrees
 *  Return: obj_width[index], -1 if the index is OutOfBounds
 */
int get_obj_width(int index){
    if(index < 0 || index >= n_objects) return -1;
    return obj_width[index];
}

