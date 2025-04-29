/*
 * scanner.c
 *
 *  Created on: Apr 18, 2023
 *      Author: dkay
 */

#include "scanner.h"

void find_objects(oi_t *sensor_data, int min, int max, int interval) {
    found_object objects[20] = {0};

    int pos = min;
    int num_scans = 3;
    float minIR = 12;
    float maxIR = 65;
    int wait_time = 50;
    int i, j, k;
    float ping_distance[180] = {0};
    float IR_distance[180] = {0};
    int size = (max - min) / interval;

    char buffer[20] = {0};

    // Ping
    for (i = 0; i < size; i++) {
        servo_move(pos);
        timer_waitMillis(wait_time);

        for (j = 0; j < num_scans; j++) {
            ping_trigger();
            ping_distance[i] += ping_getDistance();
        }

        ping_distance[i] /= num_scans;
        pos += interval;
    }

    int flag = 0;
    int index = -5;
    // Eliminate scans out of IR range and Singular width objects
    for (i = 0; i < size; i++) {
        if (flag) {
            index = i;
            flag = 0;
        }

        if (ping_distance[i] < minIR || ping_distance[i] > maxIR) {
            ping_distance[i] = 0.0;

            if (index == i - 1) {
                ping_distance[i-1] = 0.0;
            }

            flag = 1;
        }
    }

    pos = min;

    // IR Scan
    for (i = 0; i < size; i++) {
        if (ping_distance[i] != 0) {
            servo_move(pos);
            timer_waitMillis(wait_time + 25);

            for (j = 0; j < num_scans + 2; j++) {
                IR_distance[i] += adc_read();
            }

            IR_distance[i] /= num_scans + 2;
            IR_distance[i] = 60926 * pow(IR_distance[i], -1.079);
        }
        pos += interval;
    }
    pos = min;

    flag = 0;
    index = -5;
    //Eliminate scans out of IR range and Singular width objects
    for (i = 0; i < size; i++) {
        if (flag) {
            index = i;
            flag = 0;
        }

        if (IR_distance[i] < minIR || IR_distance[i] > maxIR) {
            ping_distance[i] = 0.0;
            IR_distance[i] = 0.0;

            if (index == i - 1) {
                ping_distance[i-1] = 0.0;
                IR_distance[i-1] = 0.0;
            }

            flag = 1;
        }
    }

/*
    float temp_IR[180] = {0};
    for (i = 0; i < size; i++) {
        temp_IR[i] = IR_distance[i];
    }*/


    int object = 0;

    int count_remove = 0;
    float total_IR = 0;
    float threshold = .3; //Removing too much info potentially
    int edited_flag = 0;

    for(i = 0; i < size - 1; i++) {
        if (IR_distance[i] != 0) {
            found_object curr_object = {0};

            curr_object.id = object + 1;
            curr_object.angle_min = i * interval;

            while(!(IR_distance[i] == 0 && IR_distance[i + 1] == 0) && i < size - 1) {
                if (IR_distance[i] == 0) {
                    count_remove++;
                }

                i++;
            }
            i--;

            curr_object.angle_max = i * interval;
            curr_object.angle_width = curr_object.angle_max - curr_object.angle_min;
            //&& curr_object.angle_min > min + interval && curr_object.angle_max < max - interval
            if(curr_object.angle_width > 2) {
                curr_object.angle_mid = (int)((curr_object.angle_min + curr_object.angle_max) / 2);

                for (j = (curr_object.angle_min / interval); j < (curr_object.angle_max / interval); j++) {
                    total_IR += IR_distance[j];
                }

                curr_object.distance_IR = total_IR / (((curr_object.angle_max - curr_object.angle_min) / interval) - count_remove);
                total_IR = 0;

                curr_object.actual_width = curr_object.distance_IR * tan((curr_object.angle_width / 2.0) * (M_PI/180)) * 2.0;

                for (k = (curr_object.angle_min / interval); k <= (curr_object.angle_max / interval); k++) {
                    if ((fabs(IR_distance[k] - curr_object.distance_IR)) > (curr_object.distance_IR * threshold)) {
                        IR_distance[k] = 0.0;
                        edited_flag = 1;
                    }
                }

                if (edited_flag) {
                    i = (curr_object.angle_min / interval) - 1; //Reset i
                    edited_flag = 0;
                    continue;
                }

                if(curr_object.actual_width > 2.5) {
                    objects[object].id = curr_object.id;
                    objects[object].angle_min = curr_object.angle_min;
                    objects[object].angle_max = curr_object.angle_max;
                    objects[object].angle_width = curr_object.angle_width;
                    objects[object].actual_width = curr_object.actual_width;
                    objects[object].angle_mid = curr_object.angle_mid;
                    objects[object].distance_IR = curr_object.distance_IR;
                    object++;
                }
            }
            i++;
        }
    }

    /*
    char temp[20] = {0};
    for (i = 0; i < size; i++) {
        sprintf(temp, "\n\r%d, %0.3f, %0.3f", i * interval, temp_IR[i], IR_distance[i]);
        uart_sendStr(temp);
    }
    */

    float a;
    float d;
    float o;
    float n;
    float n_total;
    float a_prime;
    float d_prime;

    /*
    //USE TRIG BEFORE
    for (i = 0; i < object; i++) {
        if(objects[i].angle_mid > 90) {
            a = (180 - objects[i].angle_mid) * (3.1415/180);
            d = objects[i].distance_IR;

            o = fabs(d * cos(a));
            n = fabs(d * sin(a));

            n_total = n + 12;

            a_prime = 3.1415 - atan(n_total / o);
            d_prime = sqrt((o * o) + (n_total * n_total));
        } else {
            a = (objects[i].angle_mid) * (3.1415/180);
            d = objects[i].distance_IR;

            o = fabs(d * cos(a));
            n = fabs(d * sin(a));

            n_total = n + 12;

            a_prime = atan(n_total / o);
            d_prime = sqrt((o * o) + (n_total * n_total));
        }

        //Adjust values
        objects[i].angle_mid = a_prime * (180.0 / 3.1415);
        objects[i].distance_IR = d_prime;
    }*/

    for (i=0; i < object; i++)
    {
        if (objects[i].actual_width < 10.1) {
            gui_add(1, objects[i].angle_mid, objects[i].actual_width, objects[i].distance_IR);
        } else {
            gui_add(0, objects[i].angle_mid, objects[i].actual_width, objects[i].distance_IR);
        }
    }

    return;
}


