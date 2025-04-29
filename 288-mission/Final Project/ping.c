/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping.h"
#include "Timer.h"

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile int STATE = 0;
//low = 0
//high = 1

void ping_init (void){
    SYSCTL_RCGCGPIO_R |= 0x2;
    while((SYSCTL_PRGPIO_R & 0x8) == 0){};
    GPIO_PORTB_DIR_R |= 0x8;
    GPIO_PORTB_DEN_R |= 0x8;
    GPIO_PORTB_AFSEL_R |= 0x8;
    GPIO_PORTB_PCTL_R &= ~0xF000;
    GPIO_PORTB_PCTL_R |= 0x7000;

    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable();

    NVIC_EN1_R |= 0x10;

    SYSCTL_RCGCTIMER_R |= 0x8;
    while((SYSCTL_PRTIMER_R & 0x8) == 0){};
    TIMER3_CTL_R &= ~0xD00;
    TIMER3_CFG_R |= 0x4;
    TIMER3_TBMR_R |= 0b0111;
    TIMER3_TBMR_R &= ~0b10000;
    TIMER3_TBPR_R |= 0xFF;
    TIMER3_TBILR_R |= 0xFFFF;
    TIMER3_CTL_R |= 0xD00;
}

void ping_trigger (void){
    STATE = 0;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x100;
    TIMER3_IMR_R &= ~0x400;
    //Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0x8;
    GPIO_PORTB_DIR_R |= 0x8;

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
    GPIO_PORTB_DATA_R &= ~0x8;
    timer_waitMicros(10);
    GPIO_PORTB_DATA_R |= 0x0008;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= ~0x8;


    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x400;
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x0008;
    TIMER3_IMR_R |= 0x00000400;
    TIMER3_CTL_R |= 0x00000100;
}

void TIMER3B_Handler(void){
    if (TIMER3_MIS_R & 0x00000400) {
        if (STATE == 0) {
            START_TIME = TIMER3_TBR_R & 0xFFFFFF;
            STATE = 1;
        } else if (STATE == 1) {
            END_TIME = TIMER3_TBR_R & 0xFFFFFF;
            STATE = 0;
        }
        TIMER3_ICR_R |= 0x00000400;  // clear the interrupt flag
    }
  // YOUR CODE HERE
  // As needed, go back to review your interrupt handler code for the UART lab.
  // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
  // includes checking the source of the interrupt and clearing the interrupt status bit.
  // Checking the source: test the MIS bit in the MIS register (is the ISR executing
  // because the input capture event happened and interrupts were enabled for that event?
  // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
  // The rest of the code in the ISR depends on actions needed when the event happens.

}

float ping_getDistance (void){
    //return (float)(START_TIME - END_TIME)/16000000/2*343*100;
    double clockCycles=START_TIME-END_TIME;

    //16mHz=16,000,000 cycles/second
    //1 cycle=1 second/16,000,000 = .0625 microseconds
    double time = clockCycles * 0.0000000625; //time in microseconds

    //seconds = 1/

    //speed of sound: 343 m/s
    double distance = time*343;
    return (float)(distance * 100 / 2.0);
}
