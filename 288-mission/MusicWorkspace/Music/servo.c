#include "servo.h"

#include "Timer.h"

#include <stdio.h>
#include <stdlib.h>

void servo_init()
{
    SYSCTL_RCGCGPIO_R |= 0x2;
    while ((SYSCTL_PRGPIO_R & 0x02)==0)
    {
    };  // 3 for stabilization
    SYSCTL_RCGCTIMER_R |= 0x2;
//

    GPIO_PORTB_DIR_R |= 0x20;
    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_DEN_R |= 0x20;
    GPIO_PORTB_PCTL_R |= 0x700000;

    TIMER1_CTL_R &= 0xFEFF;
    TIMER1_CFG_R = 0x04;
    TIMER1_TBMR_R |= 0x0A; //Moves
//    TIMER1_CTL_R &= 0xBFFF; //?
    TIMER1_TBPR_R |= (0x4E200 >> 16) & 0xFF;
    TIMER1_TBILR_R |= 0xE200 & 0xFFFF;

//    TIMER1_TBPMR_R |= 0x4E200 >> 16;
//    TIMER1_TBMATCHR_R |= 0x4E200 & 0xFFFF;

    TIMER1_CTL_R |= 0x100;
}

//1 Clock Cycle = 0.0000625 ms
//16,000 cycles/ms

void servo_move(int degrees)
{
    int val = (float) (1 + ((float) degrees / 180)) * 16000;
    TIMER1_TBPMR_R |= (320000 - val) >> 16;
    TIMER1_TBMATCHR_R |= (320000 - val) & 0xFFFF;
}
