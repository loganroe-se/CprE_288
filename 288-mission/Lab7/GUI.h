#ifndef GUI_H_
#define GUI_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "uart-interrupt.h"
#include <inc/tm4c123gh6pm.h>

void gui_add(int type, float angle, float width, float distance);
void gui_move(float angle, float distance);
void gui_send(char* json);

#endif
