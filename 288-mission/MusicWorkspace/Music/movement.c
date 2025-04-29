/*
 * movement.c
 *
 *  Created on: Jan 31, 2023
 *      Author: mbrammer
 */

#include "open_interface.h"
#include "movement.h"
#include "Timer.h"
#include<math.h>
#include "uart-interrupt.h"

#include<stdlib.h>

int forward(oi_t*sensor_data, double distance_mm) {
    timer_init();
    lcd_init();

    double dist_traveled=0;


    oi_setWheels(50,50);

    while(dist_traveled<distance_mm) {
        dist_traveled+=sensor_data->distance;
        oi_update(sensor_data);
    }

    oi_setWheels(0,0);

    return 0;
}

double move_forward(oi_t*sensor_data, double distance_mm, int angle, int edgeAngle, double edge_distance_mm){

    timer_init();
    lcd_init();

    int i=0;

    float cosine;

    float sine;

    double move=0;

    char message[50];

    //1=left; 2=right; 3=both
    int triggered=0;

    int adjustedAngle=0;
    int adjustedEdgeAngle=0;

    double sum = 0;
    oi_setWheels(250,250);

    sprintf(message,"Started\n\r");
    uart_sendStr(message);

    while(sum < distance_mm-40){
        if(sensor_data->bumpLeft==1 || sensor_data->bumpRight==1) {
            //Bump
            if(sensor_data->bumpLeft==1 && sensor_data->bumpRight==0) {
                triggered=1;
            }
            else if(sensor_data->bumpLeft==0 && sensor_data->bumpRight==1) {
                triggered=2;
            }
            else {
                triggered=3;
            }


             if((int)edge_distance_mm==0) {

                //Go backwards 150 mm

                oi_setWheels(-200, -200);

                while(move<abs(sum)) {
                    oi_update(sensor_data);
                    move+=abs(sensor_data->distance);
                }

                oi_update(sensor_data);
                move=0;

                //turn

                if(triggered==1) {
                    turn_right(sensor_data, (int)((double)(5.0/9)*angle));
                    adjustedAngle=angle;

                }
                else if(triggered==2) {
                    turn_left(sensor_data, (int)((double)(5.0/9)*(180-angle)));
                    adjustedAngle=180-angle;
                }
                else if(triggered==3) {
                    if(angle>90) {
                        turn_left(sensor_data, (int)((double)(5.0/9)*(180-angle)));
                        adjustedAngle=180-angle;
                    }
                    else if(angle<=90) {
                        turn_right(sensor_data, (int)((double)(5.0/9)*angle));
                        adjustedAngle=angle;
                    }
                }

                oi_update(sensor_data);

                //forward

                oi_setWheels(200,200);

                move=0;

                oi_update(sensor_data);

                cosine=(float)(1.5*(distance_mm*cos(((double)adjustedAngle)*(M_PI/180))));

                sprintf(message,"Cosine: %f\n\r",cosine);
                uart_sendStr(message);

                while(move<cosine+30) {
                    sprintf(message,"%lf\n\r",move);
                    uart_sendStr(message);
                    move+=abs(sensor_data->distance);
                    oi_update(sensor_data);
                }

                move=0;

                //turn opposite

                if(triggered==1 || triggered==3) {
                    turn_left(sensor_data, 65);
                }
                else {
                    turn_right(sensor_data, 65);
                }

                oi_update(sensor_data);

                oi_setWheels(200,200);

                sum=0;

                sine=(float)(distance_mm*sin((double)adjustedAngle*(M_PI/180)));

                sprintf(message,"Sine: %f\n\r",sine);
                uart_sendStr(message);

                distance_mm=sine+10;

                move=0;

                while(move<distance_mm+20) {
                    sprintf(message,"%lf %lf\n\r",move,distance_mm);
                    uart_sendStr(message);

                    move+=abs(sensor_data->distance);
                    oi_update(sensor_data);
                }

                break;
             }

             else {
                 oi_setWheels(-200, -200);

                 if(edgeAngle>90) {
                     adjustedEdgeAngle=180-edgeAngle;
                 }
                 else {
                     adjustedEdgeAngle=edgeAngle;
                 }

                 sine=(float)(edge_distance_mm*sin((double)adjustedEdgeAngle*(M_PI/180)));

                 sprintf(message,"Sine: %f\n\r",sine);
                 uart_sendStr(message);

                 //Clears edge backwards

                 if(sine<160) {
                     while(move<(int)sine+50) {
                         oi_update(sensor_data);
                         move+=abs(sensor_data->distance);
                     }
                 }
                 else {
                     while(move<150) {
                         oi_update(sensor_data);
                         move+=abs(sensor_data->distance);
                     }
                 }

                 //turn perpendicular to edge

                 if(edgeAngle>90) {
                     turn_left(sensor_data, (int)((double)(6.0/9)*(180-angle)));
                 }
                 else {
                     turn_right(sensor_data, (int)((double)(6.0/9)*(angle)));
                 }

                 //go forward
                 oi_setWheels(200,200);
                 move=0;

                 cosine=(float)(edge_distance_mm*cos((double)adjustedEdgeAngle*(M_PI/180)));
                 sprintf(message,"Cosine: %f\n\r",cosine);
                 uart_sendStr(message);

                 if(edgeAngle>90) {
                     while(move<(int)cosine+275) {
                         oi_update(sensor_data);
                         move+=abs(sensor_data->distance);
                     }
                 }

                 else {
                     while(move<(int)cosine+300) {
                          oi_update(sensor_data);
                          move+=abs(sensor_data->distance);
                      }
                 }

                 //turn in direction of target
                 if(edgeAngle>90) {
                     turn_right(sensor_data, 60);
                 }
                 else {
                     turn_left(sensor_data, 55);
                 }

                 oi_setWheels(200,200);
                 move=0;

                 sine=(float)(edge_distance_mm*sin((double)adjustedEdgeAngle*(M_PI/180)));
                 sprintf(message,"Sine: %f\n\r",sine);
                 uart_sendStr(message);


                 while(move<(int)sine+400) {
                     oi_update(sensor_data);
                     move+=abs(sensor_data->distance);
                 }

                 //turn towards target

                 if(edgeAngle>90) {
                     turn_right(sensor_data,70);
                 }
                 else if(edgeAngle<90) {
                     turn_left(sensor_data,70);
                 }

                 oi_setWheels(200,200);
                 move=0;

                 cosine=(float)(((edge_distance_mm*cos((double)adjustedEdgeAngle*(M_PI/180)))+50)-(distance_mm*cos((double)angle*(M_PI/180))));
                 sprintf(message,"Cosine: %f\n\r",cosine);
                 uart_sendStr(message);

                 if(edgeAngle>90) {
                     while(move<(int)cosine+50) {
                          oi_update(sensor_data);
                          move+=abs(sensor_data->distance);
                     }
                 }

                 else {
                     while(move<(int)cosine+50) {
                         oi_update(sensor_data);
                         move+=abs(sensor_data->distance);
                     }
                 }

                 oi_setWheels(0,0);


                 oi_update(sensor_data);
                 move=0;

                 break;
             }
        }

        oi_update(sensor_data);
        sum += sensor_data -> distance;
    }

    return 0;

}

double move_backward(oi_t*sensor_data, double distance_mm){

    timer_init();
    lcd_init();

    int i=0;

    double move=0;

    //1=left; 2=right; 3=both
    int triggered=0;

    double sum = 0;
    oi_setWheels(-100,-100);

    while(sum < distance_mm){
        oi_update(sensor_data);
        sum += abs(sensor_data -> distance);
    }

    return 0;

}

void turn_right(oi_t *sensor, double degrees){
    timer_init();
      lcd_init();

      double degrees_turned =0;
      oi_setWheels(-200,200);
      while(degrees_turned < degrees){
          oi_update(sensor);
          degrees_turned += abs(sensor -> angle);
      }



}

void turn_left(oi_t *sensor, double degrees){
    timer_init();
          lcd_init();

          double degrees_turned =0;
          oi_setWheels(200,-200);
          while(degrees_turned < degrees){
              oi_update(sensor);
              degrees_turned += abs(sensor -> angle);
          }


}



