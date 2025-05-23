/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping.h"
#include "Timer.h"

#include <stdio.h>
#include <stdlib.h>

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
//volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse

volatile int running = 0;

void ping_init (void){

  // YOUR CODE HERE
    SYSCTL_RCGCGPIO_R |= 0x2;
    SYSCTL_RCGCTIMER_R |= 0x08;

    NVIC_EN1_R |= 0x00000010;
    NVIC_PRI9_R = 0x00E0;

    while((SYSCTL_PRGPIO_R&0x10) != 0x10){};  // 3 for stabilization
    GPIO_PORTB_DIR_R |= 0x08;
    GPIO_PORTB_AFSEL_R |= 0x08;
    GPIO_PORTB_DEN_R |= 0x08;
    GPIO_PORTB_PCTL_R |= 0x7000;
    //GPIO_PORTB_AMSEL_R |= 0x08;

    GPIO_PORTB_IM_R |= 0x08;

    GPIO_PORTB_IBE_R |= 0x08;
    GPIO_PORTB_IBE_R |= 0x08;

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    // Configure and enable the timer
    TIMER3_CTL_R &= ~0x0100;

    TIMER3_CFG_R |= 0x4;

    TIMER3_TBMR_R |= 0x07;
    TIMER3_TBMR_R &= 0xEF;

    TIMER3_TBMR_R |= 0x200;

    TIMER3_CTL_R |= 0x0C00;

    TIMER3_TBPR_R |= 0xFF; //?
    TIMER3_TBILR_R |= 0xFFFF; //?

    TIMER3_IMR_R |= 0xC00;
    TIMER3_ICR_R |= 0x400;

    TIMER3_CTL_R |= 0x0100;

    SYSCTL_PRTIMER_R |= 0x8;
}

void ping_trigger (void) {
    STATE = LOW;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x100;
    TIMER3_IMR_R &= ~0xC00;
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0x08;

    GPIO_PORTB_DATA_R &= 0xF7;

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
    STATE = HIGH;
    GPIO_PORTB_DATA_R |= 0x08;

    STATE = LOW;
    GPIO_PORTB_DATA_R &= 0xF7;

    //GPIO_PORTB_AFSEL_R &= 0x08;


    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x400;
    // Re-enable alternate function, timer interrupt, and timer
    TIMER3_TBMR_R |= 0x200;

    GPIO_PORTB_AFSEL_R |= 0x08;
    TIMER3_IMR_R |= 0xC00;
    TIMER3_CTL_R |= 0x100;

    STATE = DONE;
}

void TIMER3B_Handler(void){

  // YOUR CODE HERE
  // As needed, go back to review your interrupt handler code for the UART lab.
  // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
  // includes checking the source of the interrupt and clearing the interrupt status bit.
  // Checking the source: test the MIS bit in the MIS register (is the ISR executing
  // because the input capture event happened and interrupts were enabled for that event?
  // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
  // The rest of the code in the ISR depends on actions needed when the event happens

    //running++;


    if((TIMER3_RIS_R & 0x400)==0x400) {
        if(running==0) {
            running=1;
            START_TIME=TIMER3_TBR_R;
        }

        else if(running==1) {
            running=0;
            END_TIME=TIMER3_TBR_R;
        }

        //running++;

        TIMER3_ICR_R |= 0x400;
    }

}

float ping_getDistance (void){

    // YOUR CODE HERE

    //speed=16mHz
    unsigned long clockCycles=START_TIME-END_TIME;

    //16mHz=16,000,000 cycles/second
    //1 cycle=1 second/16,000,000 = .0625 microseconds
    float time = clockCycles*.0625; //time in microseconds

    //seconds = 1/

    time *= 0.000001; //time in seconds

    //speed of sound: 343 m/s
    float distance = time*343;

    return distance;


}
