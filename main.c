/*
 * main.c
 *
 *  Created on: Mar 26, 2018
 *      Author: mhoppe & kelange
 */

#include "servo.h"
#include "Timer.h"
#include "lcd.h"
#include "button.h"
#include "uart.h"
//#include "WIFI.h"
#include "IR.h"
#include "ping.h"
#include "obj_dect.h"
#include <math.h>
static char scan_increment = 1;

//int estimate_distance(int binary_value);
//int[] rolling_average(int[] data);

int estimate_distance(int binary_value){
   return (int)(108105 / pow(binary_value, 1.166));
}


void main(void){
//    WiFi_start("letsgochamp");
//    button_init();
    lcd_init();
    servo_init();
    uart_init();

    ping_init();
    ir_init();

    /*database of cybots*/
    //int offset = -14; //cybot 2
    int offset = 47; //cybot 14


    int range_top = 180;
    turn_to(0 + offset);

    int irArray[180];
    float sonarArray[180];
    char string[20];

    timer_waitMillis(1000);
    uart_sendStr("Angle\t\tIR Sensor\tSonar Sensor\n");
        int j = 0;
        int i;for(i = offset; i < range_top + offset;i += scan_increment){
            if (i >= offset && i <= (range_top+offset)){
                //turn to i
                turn_to(i);
                //pulse,
                pulse();
                //wait for pulse
                timer_waitMillis(100);
                //find distance using distance method
                float sonar_scan = distance();

                //initiate SS0 conversion
                ADC0_PSSI_R=ADC_PSSI_SS0;
                //wait for ADC conversion to be complete
                while((ADC0_RIS_R & ADC_RIS_INR0) == 0){}
                //grab result
                int value = ADC0_SSFIFO0_R;
                int ir_scan = estimate_distance(value);
                sprintf(string, "%d\t\t%d\t\t%0.2f\n", i - offset, ir_scan, sonar_scan);
                uart_sendStr(string);
                irArray[j] = ir_scan;
                sonarArray[j] = sonar_scan;
                j++;
            }
        }

        obj_init(irArray, sonarArray, 180);
        obj_run();

        for(i = 0; i < get_n_objects();i++){
            sprintf(string, "%d\t%0.2f\t%d\t%d\n", i, get_obj_dist(i), get_obj_location(i), get_obj_width(i));
            uart_sendStr(string);
        }

        timer_waitMillis(300);
//        turn_to(obj_location[smallest] + offset);
//        timer_waitMillis(750);

    while(1);
}
