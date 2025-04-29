/*
 * Servo.c
 *
 *  Created on: Apr 4, 2023
 *      Author: dkay
 */
#include "Servo.h"

void servo_init(void) {
    SYSCTL_RCGCGPIO_R |= 0x2;
    while((SYSCTL_PRGPIO_R & 0x2) == 0){};
    GPIO_PORTB_DIR_R |= 0x20;
    GPIO_PORTB_DEN_R |= 0x20;
    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R &= ~0x00F0000;
    GPIO_PORTB_PCTL_R |= 0x00700000;

    SYSCTL_RCGCTIMER_R |= 0x2;
    while((SYSCTL_PRTIMER_R & 0x2) == 0){};
    TIMER1_CTL_R &= 0xBEFF;
    TIMER1_CFG_R |= 0x4;

    TIMER1_TBMR_R |= 0b1010;
    TIMER1_TBMR_R &= ~0b10101;
    TIMER1_TBPR_R |= 0x04;
    TIMER1_TBILR_R |= 0xE200;
    // Check this if it doesn't work
    TIMER1_TBPMR_R &= 0x00;
    TIMER1_TBPMR_R |= 0x04;
    TIMER1_TBMATCHR_R &= 0x0000;
    TIMER1_TBMATCHR_R |= 0xAB50;
    TIMER1_CTL_R |= 0xD00;
}

void servo_move(uint16_t degrees) {
    uint32_t y = (-155.555555 * degrees) + 320000;
    TIMER1_CTL_R &= 0xBEFF;
    TIMER1_TBPMR_R &= 0x00;
    TIMER1_TBPMR_R |= (y >> 16);
    TIMER1_TBMATCHR_R &= 0x0000;
    TIMER1_TBMATCHR_R |= y;
    TIMER1_CTL_R |= 0xD00;
}

