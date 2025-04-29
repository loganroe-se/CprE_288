/**
 * lab5_template.c
 *
 * Template file for CprE 288 Lab 5
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 *
 * @author Phillip Jones, updated 6/4/2019
 * @author Diane Rover, updated 2/25/2021, 2/17/2022
 */

#include "Timer.h"
#include "open_interface.h"
#include "lcd.h"
#include "servo.h"
#include "music.h"


int main(void)
{
    //timer_init(); // Must be called before lcd_init(), which uses timer functions
    //lcd_init();
    //servo_init();


    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    oi_uartInit();

    int i,j;

    playMusic();

    //while(1) {
//    servo_move(0);
//    timer_waitMillis(100);
//    servo_move(90);
//    timer_waitMillis(100);
//    servo_move(30);
    //servo_move(150);
    //timer_waitMillis(100);

    //    servo_move(90);

    // }
    oi_free(sensor_data);

    return 0;

}
