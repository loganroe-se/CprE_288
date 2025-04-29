#include "GUI.h"


void gui_add(int object_type, float angle, float distance){
    char json[200] = {0};

    char o_type[10] = {0};

    switch(object_type){
        case 0:
                    strcpy(o_type, "tall");
                    break;
        case 1:
                    strcpy(o_type, "goal");
                    break;
        case 2:
                    strcpy(o_type, "short");
                    break;
        case 3:
                    strcpy(o_type, "line");
                    break;
        case 4:
                    strcpy(o_type, "hole");
                    break;
        default:
                    strcpy(o_type, "error");
                    break;
    }

    sprintf(json, "{\"function\":\"add\",\"type\":\"%s\",\"angle\":%0.3f,\"distance\":%0.3f}", o_type, angle, distance);
    gui_send(&json[0]);
}

void gui_move(float angle, float distance){
    char json[200] = {0};

    sprintf(json, "{\"function\":\"move\",\"angle\":%0.3f,\"distance\":%0.3f}", angle, distance);
    gui_send(&json[0]);
}

void gui_send(char* json){
    uart_sendStr(json);
}
