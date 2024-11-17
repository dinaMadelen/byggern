#include "solenoid.h"
extern CanMsg message;
extern uint16_t COUNTER;


void SOLENOID_init(void){
    PIOB->PIO_PER |= PIO_PB20; //enable PIO for PC22
    PIOB->PIO_OER |= PIO_PB20; //enable output for PC22
    PIOB->PIO_SODR |= PIO_PB20; //set output to low
}

void SOLENOID_fire(void){
    COUNTER++;
    if((message.id) == 1){
        PIOB->PIO_CODR |= PIO_PB20; //set output to high
        
    }
    if(COUNTER > 5000){
        PIOB->PIO_SODR |= PIO_PB20;
        COUNTER = 0;
    }
}

