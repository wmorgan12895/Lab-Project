/*
 * main.c
 *
 *  Created on: Mar 26, 2018
 *      Author: mhoppe & kelange
 */
#include "movement.h"
#include "open_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "servo.h"
#include "Timer.h"
#include "lcd.h"
#include "button.h"
#include "uart.h"
#include "IR.h"
#include "ping.h"
#include "obj_dect.h"
#include <math.h>

void doCommand(char cmd, oi_t *sensor){
    switch(cmd) {
    case 'w' :
        move_forward_with_bumps(sensor, 100);
        uart_sendStr("Moved Forward 10cm\n\r");
        break;
    case 's' :
        move_backwards(sensor, 100);
        uart_sendStr("Moved Backward 10cm\n\r");
        break;
    case 'd' :
        turn_clockwise(sensor, 90);
        uart_sendStr("Turned clockwise 90 degrees\n\r");
        break;
    case 'a' :
        turn_counterclockwise(sensor, 90);
        uart_sendStr("Turned counter-clockwise 90 degrees\n\r");
        break;
    case 'e' :
       turn_clockwise(sensor, 45);
       uart_sendStr("Turned counter-clockwise 45 degrees\n\r");
       break;
    case 'q' :
       turn_counterclockwise(sensor, 45);
       uart_sendStr("Turned counter-clockwise 45 degrees\n\r");
       break;
    case '1' :
        uart_sendStr("Function 1 (180 degree sweep?) \n\r");
        obj_init();
        obj_scan();
        obj_run();
        break;
    case '2' :
        uart_sendStr("Function 2 (360 degree sweep?)\n\r");
        break;
    case '3' :
        uart_sendStr("Function 3 (flash LED)\n\r");
        oi_setLeds(1,1,0,255);
        timer_waitMillis(300);
        oi_setLeds(0,0,0,0);
        timer_waitMillis(300);
        oi_setLeds(1,1,0,255);
        timer_waitMillis(300);
        oi_setLeds(0,0,0,0);
        timer_waitMillis(300);
        oi_setLeds(1,1,0,255);
        timer_waitMillis(300);
        oi_setLeds(0,0,0,0);
        oi_setLeds(1,1,0,255);
        timer_waitMillis(300);
        oi_setLeds(0,0,0,0);
        oi_setLeds(1,1,0,255);
        timer_waitMillis(300);
        oi_setLeds(0,0,0,0);
        oi_setLeds(1,1,0,255);
        timer_waitMillis(300);
        oi_setLeds(0,0,0,0);
        break;
    case '4' :
        uart_sendStr("Function 4 (?)\n\r");
        break;
    }

}
//putty wifi 192.168.1.1 port 288 raw pass cpre288psk

int main(void) {
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    oi_free(sensor_data);
    lcd_init();
    button_init();
    uart_init();

    //WiFi_start("password");

    lcd_printf("Ready to connect");
    oi_setWheels(0, 0);
    char cmd = 0;

    while(1){
        if(cmd != 13){
            uart_sendStr("Awaiting Command\n\r");
        }
        cmd = uart_receive();
        doCommand(cmd, sensor_data);
    }
}


//void main(void){
//    lcd_init();
//    uart_init();
//
//    obj_init();
//    obj_scan();
//    obj_run();
//
//    char string[20];
//    int i;for(i = 0; i < get_n_objects();i++){
//        sprintf(string, "%d\t%0.2f\t%d\t%d\n", i, get_obj_dist(i), get_obj_location(i), get_obj_width(i));
//        uart_sendStr(string);
//    }
//
//    timer_waitMillis(300);
////        turn_to(obj_location[smallest] + offset);
////        timer_waitMillis(750);
//
//    while(1);
//}
