/*
 * movement.c
 *
 *  Created on: Jan 23, 2018
 *      Author: wrmorgan
 */
#include "lcd.h"
#include "open_interface.h"
#include "uart.h"

void move_forward(oi_t *sensor, int centimeters){
    //Pass positive numbers for centimeters
    int sum = 0;
    oi_setWheels(100, 100); // move forward; full speed
    while (sum < centimeters) {
        oi_update(sensor);
        sum += sensor->distance;
    }
    oi_setWheels(0, 0);
}

void move_backwards(oi_t *sensor, int centimeters){
    //Pass positive numbers for centimeters
    lcd_init();
    int sum = centimeters;
    oi_setWheels(-100, -100);
    while (sum > 0) {
            oi_update(sensor);
            sum += sensor->distance;
    }
    oi_setWheels(0, 0);
}

void turn_clockwise(oi_t *sensor, int degrees){
    //Pass positive numbers for degrees
    int sum = degrees;
        oi_setWheels(-75, 75);
        timer_waitMillis(20);
        while (sum > 17) {
                oi_update(sensor);
                sum += sensor->angle;
        }
        oi_setWheels(0, 0);
}

void turn_counterclockwise(oi_t *sensor, int degrees){
    //Pass positive numbers for degrees
    int sum = 0;
        oi_setWheels(75, -75);
        while (sum < degrees-8) {
                oi_update(sensor);
                sum += sensor->angle;
         }
        oi_setWheels(0, 0);
}


void move_forward_with_bumps(oi_t *sensor, int centimeters){
    int sum = 0;
    oi_setWheels(50, 50);
    while (sum < centimeters) {
            oi_update(sensor);
            sum += sensor->distance;
           if(sensor->bumpLeft) {
              uart_sendStr("Bump left sensor\r\n");
              break;
           } else if (sensor->bumpRight) {
              uart_sendStr("Bump right sensor\r\n");
              break;
           } else if (sensor->cliffFrontLeft){
               uart_sendStr("Cliff front left\r\n");
               break;
           } else if (sensor->cliffFrontRight){
               uart_sendStr("Cliff front right\r\n");
               break;
           } else if (sensor->cliffLeft){
               uart_sendStr("Cliff left\r\n");
               break;
           } else if (sensor->cliffRight){
               uart_sendStr("Cliff right\r\n");
               break;
           } else if (sensor->cliffFrontRightSignal >= 2600){
               uart_sendStr("Boundary Cliff front right\r\n");
               break;
           } else if (sensor->cliffRightSignal >= 2600){
               uart_sendStr("Boundary Cliff Right\r\n");
               break;
           } else if (sensor->cliffFrontLeftSignal >= 2600){
               uart_sendStr("Boundary Cliff front left\r\n");
               break;
           } else if (sensor->cliffLeftSignal >= 2600){
               uart_sendStr("Boundary Cliff left\r\n");
               break;
           }
    }
    lcd_printf("R:%d FR:%d L:%d FL:%d",sensor->cliffRightSignal, sensor->cliffFrontRightSignal, sensor->cliffLeftSignal, sensor->cliffFrontLeftSignal );
    oi_setWheels(0, 0);
}
