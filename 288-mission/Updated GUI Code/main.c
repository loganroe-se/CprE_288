/**
 * main.c
 */

#include "open_interface.h"
#include <string.h>
#include <math.h>
#include "uart-interrupt.h"
#include "movement.h"
#include "GUI.h"

void scan_helper(oi_t *sensor_data);
void move_helper(oi_t *sensor_data,char direction);
void turn_helper(oi_t *sensor_data,char direction);
void auto_helper(oi_t *sensor_data);
double input_helper();

int main()
{
    timer_init();
    lcd_init();
    //cyBOT_SERVO_cal();

    oi_t *sensor_data = oi_alloc();

    oi_init(sensor_data);
    uart_interrupt_init();
    char buffer[100];

    char send[1];



    while(true){
        command_flag = uart_receive();//Change for GUI

        sprintf(send,"%c",command_flag);

        lcd_printf(send);

//        if(command_flag == 1) {//Scan
//            command_flag = 0;
//            scan_helper(sensor_data);
//        }

        if(command_flag == 'w') {//Move forward
            command_flag = '0';
            move_helper(sensor_data,'l');
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
//                        double distance = 10 * (point_small(sensor_data, 0, 180, 2) - 21.5);
//                        double offcourse = navigate_forward(sensor_data, distance);
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
            double distance = 10 * (point_small(sensor_data, 0, 180, 2) - 22.5);
            double offcourse = navigate_forward(sensor_data, distance);

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
    uart_sendStr("Scanning");
}

void move_helper(oi_t *sensor_data,char direction){

    char buffer[10];

    double distance = 1;

    sprintf(buffer, "%f\n", distance);
    //uart_sendStr(buffer);

    if(direction == 's'){
       move_backward(sensor_data,1);
       distance = -1;
    }else{
        move_forward(sensor_data,1);
    }


//    if(distance > 0) {
//        distance = move_forward(sensor_data, distance);
//    } else {
//        distance = move_backward(sensor_data, fabs(distance));
//    }

    gui_move(0.0, distance);
}

void turn_helper(oi_t *sensor_data, char direction){

    char buffer[10];

    double angle = 1;

    sprintf(buffer, "%f", angle);
//    uart_sendStr(buffer);

    if(direction=='r') {
        turn_right(sensor_data,1);
    }
    else {
        turn_left(sensor_data,1);
        angle=-1;
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

        double distance = 10 * (point_small(sensor_data, 0, 180, 2) - 21.5);
        double offcourse = navigate_forward(sensor_data, distance);

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

double input_helper(){
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

