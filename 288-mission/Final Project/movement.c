/*
 * movement.c
 *
 *  Created on: Feb 5, 2023
 *      Author: dkay
 */

#include "movement.h"
#include "open_interface.h"
#include "uart-interrupt.h"

typedef struct{
    int id;
    int angle_min;
    int angle_max;

    int angle_width;
    float actual_width;

    int angle_mid;
    float distance_IR;
    float distance_Ping;
} found_object;


double move_forward (oi_t *sensor_data, double distance_cm){
    //lcd_init();

    double distance_mm = 10 * distance_cm;

    double sum = 0;

    oi_setWheels(100, 100);

    while (sum < distance_mm) {
        oi_update(sensor_data);

        sum += sensor_data -> distance;

        if(sensor_data->bumpLeft || sensor_data->bumpRight){
            oi_setWheels(0,0);
            return sum / 10;
        }

        //lcd_clear();
        //lcd_printf("Distance: %.2fmm", sum);
    }

    //oi_setWheels(0,0);

    return sum / 10;
}

double move_backward (oi_t *sensor_data, double distance_cm){
    //lcd_init();

    double distance_mm = 10 * distance_cm;

    double sum = 0;

    oi_setWheels(-100, -100);

    while (sum < distance_mm) {
        oi_update(sensor_data);
        sum += -sensor_data -> distance;

        //lcd_clear();
        //lcd_printf("Distance: %.2fmm", sum);
    }

//    oi_setWheels(0,0);

    return -sum / 10;
}

double turn_left(oi_t *sensor_data, double degrees){
    double sum = 0;

    oi_setWheels(150, -150);

    while (sum < degrees) {
        oi_update(sensor_data);

        sum += sensor_data->angle;

//        lcd_clear();
//        lcd_printf("Turned: %.2f degrees", sum);
    }

    //oi_setWheels(0,0);

    return sum;
}

double turn_right(oi_t *sensor_data, double degrees){
    double sum = 0;

    oi_setWheels(-150, 150);

    while (sum < degrees) {
        oi_update(sensor_data);

        sum += -sensor_data->angle;

//        lcd_clear();
//        lcd_printf("Turned: %.2f degrees", sum);
    }

    //oi_setWheels(0,0);

    return sum;
}

double navigate_forward (oi_t *sensor_data, double distance_mm){
    double sum = 0;
    double offcourse = 0;

    while (sum < distance_mm) {
        oi_update(sensor_data);

        if (sensor_data->bumpLeft) {
            sum -= move_backward(sensor_data, 150);
            turn_right(sensor_data, 90);
            move_forward(sensor_data, 250);
            offcourse += 250;
            turn_left(sensor_data, 90);
        } else if (sensor_data->bumpRight) {
            sum -= move_backward(sensor_data, 150);
            turn_left(sensor_data, 90);
            move_forward(sensor_data, 250);
            offcourse -= 250;
            turn_right(sensor_data, 90);
        }

        sum += move_forward(sensor_data, distance_mm - sum);

//        lcd_clear();
//        lcd_gotoLine(1);
//        lcd_printf("Total Distance:");
//        lcd_gotoLine(2);
//        lcd_printf("%.2fmm", sum);
    }

    return offcourse;
}

/*
void scan_range(oi_t *sensor_data, int min, int max, int interval){
    cyBOT_init_Scan(0b0111);
    cyBOT_Scan_t* temp = calloc(1, sizeof(cyBOT_Scan_t));

    char buffer[100];

    int pos = min;

    send_putty("Degrees PING Distance (cm)");

    int i;
    for(i = 0; i < (max - min) / interval; i++) {
        cyBOT_Scan(pos, temp);

        sprintf(buffer, "%03d     %d", pos, temp->IR_raw_val);

        send_putty(buffer);

        pos = pos + interval;
    }

    return;
}
*/

/*
double point_small(oi_t *sensor_data, int min, int max, int interval){
    int num_scans = 3;

    cyBOT_init_Scan(0b0111);
    //right_calibration_value = 280000;
    //left_calibration_value = 1246000;

    cyBOT_Scan_t* temp = malloc(sizeof(cyBOT_Scan_t));


    char buffer[100];

    int pos = min;

    int angle[90];
    float distance_aggregate[90] = {0};
    float distance_avg[90] = {0};

    send_putty("Degrees PING Distance (cm)");

    int i,j;
    for(i = 0; i < (max - min) / interval; i++) {
        for(j = 0; j < num_scans; j++){
            if(command_flag == 2) {
                command_flag = 0;
                return 0.0;
            }

            cyBOT_Scan(pos, temp);
            distance_aggregate[i] += temp->IR_raw_val;
        }

        distance_avg[i] = distance_aggregate[i] / num_scans;

        sprintf(buffer, "Avg: %03d     %f", pos, distance_avg[i]);
        send_putty(buffer);

        angle[i] = pos;

        pos = pos + interval;
    }

    float max_distance = 700;

    int sizeScan = i;

    found_object* objects[20];
    int num_objects = 0;

    float threshold = .11;

    //Find objects in the field
    for (i = 1; i < sizeScan - 1; i++){
        //if(distance_avg[i] < max_distance && flag) {
        if(distance_avg[i] > max_distance && fabs(distance_avg[i-1] - distance_avg[i]) / ((distance_avg[i-1] + distance_avg[i]) / 2) < threshold) {
            found_object* curr_object = calloc(1, sizeof(found_object));

            curr_object->id = num_objects;
            curr_object->angle_min = angle[i+1];

            while (distance_avg[i] > max_distance && fabs(distance_avg[i-1] - distance_avg[i]) / ((distance_avg[i-1] + distance_avg[i]) / 2) < threshold && i < sizeScan - 1) i++;
            i--;

            curr_object->angle_max = angle[i+1];

            curr_object->angle_mid = (int)((curr_object->angle_min + curr_object->angle_max) / 2);
            curr_object->angle_width = curr_object->angle_max - curr_object->angle_min;
            curr_object->distance_IR = distance_avg[(int)(curr_object->angle_mid / interval)];

            objects[num_objects++] = curr_object;
        }
    }

    //Locate the smallest object
    int smallest_index = 0;
    float smallest_width = 1000;

    float aggregate = 0;

    for(i = 0; i < num_objects; i++) {
        if(objects[i]->angle_width >= 4){
            aggregate = 0;
            for(j = 0; j < 15; j++) {
                if(command_flag == 2) {
                    command_flag = 0;
                    return 0.0;
                }

                cyBOT_Scan(objects[i]->angle_mid, temp);

                if(temp->sound_dist > 40 && temp->sound_dist < 125){
                    aggregate += temp->sound_dist;
                } else {
                    j--;
                }

                sprintf(buffer, "PING: %03d     %f", objects[i]->angle_mid, temp->sound_dist);
                send_putty(buffer);
            }
            objects[i]->distance_Ping = aggregate / 15.0;

            sprintf(buffer, "PING AVG: %03d     %f", objects[i]->angle_mid, objects[i]->distance_Ping);
            send_putty(buffer);

            objects[i]->actual_width = objects[i]->distance_Ping * tan((objects[i]->angle_width / 2.0) * (M_PI/180)) * 2.0;

            if(objects[i]->actual_width < smallest_width) {
                smallest_index = i;
                smallest_width = objects[smallest_index]->actual_width;
            }
        }
    }

    //Choose manual or automatic


    cyBOT_Scan(objects[smallest_index]->angle_mid, temp);

    float a;
    float d;

    float o;
    float n;

    float n_total;


    float a_prime;
    float d_prime;

    if(objects[smallest_index]->angle_mid > 90) {
        a = (180 - objects[smallest_index]->angle_mid) * (3.1415/180);
        d = objects[smallest_index]->distance_Ping;

        o = fabs(d * cos(a));
        n = fabs(d * sin(a));

        n_total = n + 12;

        a_prime = 3.1415 - atan(n_total / o);
        d_prime = sqrt((o * o) + (n_total * n_total));
    } else {
        a = (objects[smallest_index]->angle_mid) * (3.1415/180);
            d = objects[smallest_index]->distance_Ping;

        o = fabs(d * cos(a));
        n = fabs(d * sin(a));

        n_total = n + 12;

        a_prime = atan(n_total / o);
        d_prime = sqrt((o * o) + (n_total * n_total));
    }

    //Point to the smallest object
    int object_location = (a_prime) * (180 / 3.1415);
    if(object_location > 90) {
        turn_left(sensor_data, object_location - 90);
    } else if (object_location < 90) {
        turn_right(sensor_data, 90 - object_location);
    }

    return d_prime;
}
*/


