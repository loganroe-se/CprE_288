/*
 * main.c
 */

#include "open_interface.h"
#include <string.h>
#include <math.h>
#include "uart-interrupt.h"
#include "movement.h"
#include "scanner.h"
#include "GUI.h"
#include "music.h"

void scan_helper(oi_t *sensor_data);
void move_helper(oi_t *sensor_data,char direction);
void turn_helper(oi_t *sensor_data,char direction);
void auto_helper(oi_t *sensor_data);
float input_helper();

int main()
{
    //TODO SERVO CALIBRATION
   oi_t *sensor_data = oi_alloc();
   //oi_free(sensor_data);
   oi_init(sensor_data);
   timer_init();
   lcd_init();
   uart_interrupt_init();
   //cyBOT_SERVO_cal();
   ping_init();
   servo_init();
   servo_move(0);
   //servo_move(180);

   adc_init();

    servo_move(0);
    char lineData[50];


    while(true){
        command_flag = uart_receive();//Change for GUI


        //sprintf(cliffData,"Cliff: %d %d %d %d",sensor_data->cliffLeft,sensor_data->cliffFrontLeft,sensor_data->cliffFrontRight,sensor_data->cliffRight);

        sprintf(lineData,"Line: %d %d %d %d",sensor_data->cliffLeftSignal,sensor_data->cliffFrontLeftSignal,sensor_data->cliffFrontRightSignal,sensor_data->cliffRightSignal);
        uart_sendStr(lineData);

        /*if(sensor_data->cliffLeftSignal>2700 || sensor_data->cliffFrontLeftSignal>2700 || sensor_data->cliffFrontRightSignal>2700 || sensor_data->cliffRightSignal>2700) {
            timer_waitMillis(500);
        }*/

//        if(command_flag == 1) {//Scan
//            command_flag = 0;
//            scan_helper(sensor_data);
//        }

        if(command_flag == 'w') {//Move forward
            //lcd_printf(cliffData);
            //uart_sendStr(cliffData);

            command_flag = '0';

            if(!sensor_data->cliffFrontLeft && !sensor_data->cliffFrontRight) {
                move_helper(sensor_data,'w');
            }

        }
        else if(command_flag == 's') {//Move backwards
            command_flag = '0';
            move_helper(sensor_data,'s');
        }

        else if(command_flag == 'd') {//Turn right
           command_flag = '0';
           turn_helper(sensor_data,'r');
       }

       else if(command_flag == 'a') {//Turn left
               command_flag = '0';
               turn_helper(sensor_data,'l');
       }

       else if(command_flag == 'p') {//Scan
              command_flag = '0';
              scan_helper(sensor_data);
              servo_move(0);
       }

       else if(command_flag == 'm') {//Scan
              command_flag = '0';
              playMusic();
       }

       else {
           oi_setWheels(0,0);
       }

//        if(command_flag == 3) {//Turn
//            command_flag = 0;
//            turn_helper(sensor_data);
//        }
//
//        if(command_flag == 4) {//Autonomous
//            command_flag = 0;
//            auto_helper(sensor_data);
//            /*
//                        command_flag = 0;
//
//                        float distance = 10 * (point_small(sensor_data, 0, 180, 2) - 21.5);
//                        float offcourse = navigate_forward(sensor_data, distance);
//
//                        if(offcourse < -1) {
//                            turn_right(sensor_data, 90);
//                            move_forward(sensor_data, fabs(offcourse));
//                            turn_left(sensor_data, 90);
//                        } else if (offcourse > 1) {
//                            turn_left(sensor_data, 90);
//                            move_forward(sensor_data, fabs(offcourse));
//                            turn_right(sensor_data, 90);
//                        }
//            */
//        }
//
//        if(command_flag == 5) {//Terminate
//            command_flag = 0;
//            break;
//        }


        /*
        int data = uart_receive();

        lcd_printf("%c", (char)(data));

        sprintf(buffer, "Got a(n) %c", (char)(data));

        send_putty(buffer);

        if(data == 'w'){
            move_forward(sensor_data,  500);
        } else if(data == 's'){
            move_backward(sensor_data, 500);
        } else if (data == 'a') {
            turn_left(sensor_data, 90);
        } else if (data == 'd') {
            turn_right(sensor_data, 90);
        } else if (data == 'm') {
            float distance = 10 * (point_small(sensor_data, 0, 180, 2) - 22.5);
            float offcourse = navigate_forward(sensor_data, distance);

            if(offcourse < -1) {
                turn_right(sensor_data, 90);
                move_forward(sensor_data, fabs(offcourse));
                turn_left(sensor_data, 90);
            } else if (offcourse > 1) {
                turn_left(sensor_data, 90);
                move_forward(sensor_data, fabs(offcourse));
                turn_right(sensor_data, 90);
            }

        } else if (data == 'h') {
            break;
        }
        */
    }

    oi_free(sensor_data);

	return 0;
}


//-------------------------Helper functions-------------------------

void scan_helper(oi_t *sensor_data){
    //uart_sendStr("Scanning");
	find_objects(sensor_data, 0, 180, 1);
    uart_sendStr("D");
}

void move_helper(oi_t *sensor_data,char direction){
    char buffer[10] = {0};

    float distance = 1;
    //uart_sendStr(buffer);

    if(direction == 's'){
       distance=move_backward(sensor_data,1);
       //distance = -1;
    }else{
        distance=move_forward(sensor_data,1);
    }


//    if(distance > 0) {
//        distance = move_forward(sensor_data, distance);
//    } else {
//        distance = move_backward(sensor_data, fabs(distance));
//    }

    gui_move(0.0, distance);

    //Check bump
    /*if(bump_flag == 3) {
        sprintf(bumpString,"\nBump Middle Triggered\n");
        uart_sendStr(bumpString);
    }

    if(bump_flag == 2) {
        sprintf(bumpString,"\nBump Left Triggered\n");
        uart_sendStr(bumpString);
    }

    if(bump_flag == 1) {
        sprintf(bumpString, "\nBump Right Triggered\n");
        uart_sendStr(bumpString);
    }
    bump_flag = 0;
    lcd_printf("Finished");
    uart_sendStr(bumpString);*/
}

void turn_helper(oi_t *sensor_data, char direction){

    char buffer[10];

    float angle = 1;

//    uart_sendStr(buffer);

    if(direction=='r') {
        angle=turn_right(sensor_data,1);
    }
    else {
        angle=-turn_left(sensor_data,1);
        //angle=-1;
    }


    /*if(angle < 90) {
        angle = 90 - turn_right(sensor_data, fabs(angle - 90));
    } else {
        angle = 90 + turn_left(sensor_data, angle - 90);
    }*/

    gui_move(angle, 0.0);
}

void auto_helper(oi_t *sensor_data){
    /*
        command_flag = 0;

         distance = 10 * (point_small(sensor_data, 0, 180, 2) - 21.5);
         offcourse = navigate_forward(sensor_data, distance);

        if(offcourse < -1) {
            turn_right(sensor_data, 90);
            move_forward(sensor_data, fabs(offcourse));
            turn_left(sensor_data, 90);
        } else if (offcourse > 1) {
            turn_left(sensor_data, 90);
            move_forward(sensor_data, fabs(offcourse));
            turn_right(sensor_data, 90);
        }
    */
}

float input_helper(){
    char input;
    char number[10] = {0};
    int count = 0;

    while(1) {
        if(command_flag) {
            input = command_flag;
            command_flag = 0;

            if(input == '=' || count > 9) {
                break;
            }

            if (input == 127) {//Backspace
                number[--count] = 0;
            } else {
                number[count++] = input;

            }
        }
    }

    number[count] = 's';

    return atof(number);
}

