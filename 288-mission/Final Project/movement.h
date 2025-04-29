/*
 * movement.h
 *
 *  Created on: Feb 5, 2023
 *      Author: dkay
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"
#include "lcd.h"
#include "ping.h"

double move_forward (oi_t *sensor_data, double distance_mm);
double move_backward (oi_t *sensor_data, double distance_mm);

double handleCollision(oi_t *sensor_data, double distance_mm);
int handleBump(oi_t *sensor_data);
int handleCliff(oi_t *sensor_data);
int handleTall(oi_t *sensor_data, double distance_mm);

double turn_left(oi_t *sensor_data, double degrees);
double turn_right(oi_t *sensor_data, double degrees);

double navigate_forward (oi_t *sensor_data, double distance_mm);

void scan_range(oi_t *sensor_data, int min, int max, int interval);
double point_small(oi_t *sensor_data, int min, int max, int interval);


#endif /* MOVEMENT_H_ */
