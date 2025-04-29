/*
 * movement.h
 *
 *  Created on: Jan 31, 2023
 *      Author: mbrammer
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"

int forward(oi_t*sensor_data, double distance_mm);
double move_forward(oi_t*sensor_data, double distance_mm, int angle, int edgeAngle, double edge_distance_mm);
double move_backward(oi_t*sensor_data, double distance_mm);
void turn_right(oi_t *sensor, double degrees);
void turn_left(oi_t *sensor, double degrees);



#endif /* MOVEMENT_H_ */
