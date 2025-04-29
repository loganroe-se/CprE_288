/*
 * scanner.h
 *
 *  Created on: Apr 18, 2023
 *      Author: dkay
 */

#ifndef SCANNER_H_
#define SCANNER_H_

#include "ping.h"
#include "Servo.h"
#include "adc.h"
#include <math.h>
#include "uart-interrupt.h"
#include "GUI.h"
#include <stdlib.h>

typedef struct{
    int id;
    int angle_min;
    int angle_max;

    int angle_width;
    float actual_width;

    float angle_mid;
    float distance_IR;
    float distance_Ping;
} found_object;

void find_objects(oi_t *sensor_data, int min, int max, int interval);

#endif /* SCANNER_H_ */
