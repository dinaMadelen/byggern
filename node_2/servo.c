#include "servo.h"
#include "can.h"
extern CanMsg message;

void PWM_setup(){

    
    PMC->PMC_PCER1 = (1<<4); //Enable clock for PIOB

    PIOB->PIO_PER = PIO_PB13; //enable PIO for PB13
    PIOB->PIO_OER = PIO_PB13; //enable output for PB13
    PIOB->PIO_PDR |= PIO_PDR_P13; // disable PIO for PB13
    PIOB->PIO_ABSR |= PIO_ABSR_P13; // enable peripheral B for PB13
    PWM->PWM_CH_NUM[1].PWM_CMR = ((0<<PWM_CMR_CPRE_Pos) & PWM_CMR_CPRE_Msk) | PWM_CMR_CPOL; //set clock polarity
    PWM->PWM_CH_NUM[1].PWM_CMR |= PWM_CMR_CPRE_MCK_DIV_128; //set clock prescaler
    //PIOB->PIO_SODR = PIO_PB13; //set output to high

    PWM->PWM_CH_NUM[1].PWM_CPRD = 13125; //period(20ms)
    //PWM->PWM_CH_NUM[1].PWM_CDTY = 1378; //duty 2.1 ms
    PWM->PWM_CH_NUM[1].PWM_CDTY = 0; //duty 0 ms

    PWM->PWM_ENA = (1<<1); //enable ch1

}

double SERVO_joystick_to_duty_cycle(uint8_t joystick_value){
    double duty_cycle_ms = 0.9 + 0.0125*(double)joystick_value;
    return duty_cycle_ms;
}

void SERVO_set_duty_cycle(double duty_cycle_ms){
    PWM->PWM_CH_NUM[1].PWM_CPRD = 13125; //period(20ms)
    double mellomledd = 84000.0/128.0;
    double dutyf = duty_cycle_ms*mellomledd; //duty to wanted value
    uint16_t duty;
    duty = (uint16_t)dutyf;
    if(duty > 1378){ //safeguard
        duty = 1378;
    }else if(duty < 591){
        duty = 591;
        }
    PWM->PWM_CH_NUM[1].PWM_CDTY = duty; //sets it
}

void SERVO_control_duty_cycle_with_joystick(){
    
    if (message.id == 5){
        uint8_t joy_pos = message.byte[0];
        double duty_cycle = SERVO_joystick_to_duty_cycle(joy_pos);
        SERVO_set_duty_cycle(duty_cycle);
    }

}
