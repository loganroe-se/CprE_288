#include "adc.h"

void adc_init(void){
    SYSCTL_RCGCADC_R |= 0x0001;
    SYSCTL_RCGCGPIO_R |= 0x0002;
    GPIO_PORTB_AFSEL_R |= 0x0010;
    GPIO_PORTB_DEN_R |= 0x0010;
    GPIO_PORTB_AMSEL_R |= 0x0010;
    ADC0_ACTSS_R |= 0x0008;
    //ADC0_RIS_R
    ADC0_IM_R |= 0x0000; //No interupts
    //ADC0_ISC_R
    ADC0_EMUX_R |= 0x0000; //ADC_PSSI_R
    ADC0_PSSI_R |= 0x0008;
    ADC0_SSMUX3_R |= 0x000A;
    //ADC0_SSCTL3_R
    //ADC0_SSFIFO3_R
    //ADC0_CC_R
    SYSCTL_PRADC_R |= 0x0001;
}

uint16_t adc_read(void){
    while (ADC0_RIS_R & 0x0008){};
    return ADC0_SSFIFO3_R;
}
