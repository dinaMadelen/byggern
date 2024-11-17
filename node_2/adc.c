#include "adc.h"
extern uint8_t goals;
extern uint16_t COUNTER_2;
extern uint8_t ADC_row_counter;
void ADC_init(){
    PMC->PMC_PCER1 |= (1 << 5); //Enable clock for ADC
    ADC->ADC_CR |= 0x0002; //Reset ADC + enable conversion
    ADC->ADC_MR = ADC_MR_FREERUN_ON; //Set ADC mode register

    ADC->ADC_CHER = (1 << 0); //Enable ADC channel 0
};


uint8_t ADC_return_IR_signal(){
    if ((ADC->ADC_CDR[0]) < 700){
        return 1;
    }else{
        return 0;
    }
}

void count_goals(){
    COUNTER_2++;
    if (ADC_return_IR_signal()){
        ADC_row_counter++;

        if((ADC_row_counter> 15)&&(COUNTER_2>5000)){
            goals++;
            COUNTER_2 = 0;
        }

    }else{
        ADC_row_counter =0;
    }
    
}


void ADC_test(void){
    //ADC->ADC_CR |= 0x0002; //Start conversion
    //while(!(ADC->ADC_ISR & 0x0001)){} //Wait for end of conversion
    printf("ADC value: %d\n\r", ADC->ADC_CDR[0]);
};