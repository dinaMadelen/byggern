#include "encoder.h"
extern CanMsg message;
extern int16_t OFFSET;
double full_length = 2826;
extern uint8_t PID_RUN_FLAG;
extern uint16_t interrupt_tid_ms;
extern uint8_t goals;

void ENCODER_init(void){
    
    PIOC->PIO_PDR |= PIO_PC25| PIO_PC26; // Disable PIO control of pins
    PIOC->PIO_ABSR |= PIO_PC25| PIO_PC26; // Enable peripheral control of pins

   

    PMC->PMC_PCER1 |= (1<<1); // Enable peripheral clock

    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0| TC_CMR_ABETRG; //| TC_CMR_ETRGEDG_RISING | // Set up TC0 channel 0 TC_CMR_TCCLKS_XC0, where X
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; // Enable clock and start counter

    //TC2->TC_CHANNEL[0].TC_RC = 0xFFFF; // Set max value
    TC2->TC_BMR = TC_BMR_QDEN | TC_BMR_POSEN ; // Enable quadrature decoder and position measure mode

    PIOC->PIO_PER |= PIO_PC23; //enable PIO for PB13
    PIOC->PIO_OER |= PIO_PC23; //enable output for PB13
   

    //TC0->TC_CHANNEL[0].TC_IER = TC_IER_COVFS;  // Enable overflow interrupt
    //NVIC_EnableIRQ(ID_TC0);  // Enable the NVIC interrupt for TC0

}

int16_t ENCODER_get_raw_position(void){
    int16_t position = TC2->TC_CHANNEL[0].TC_CV;
    return position;
}

int16_t ENCODER_get_position(void){
    int16_t position = ENCODER_get_raw_position();
    double unscaled_position = (double)(OFFSET - position);
    double true_position_db = (unscaled_position/full_length)*100.0;
    uint16_t true_position = (uint16_t)true_position_db;
    return true_position;
}

int16_t ENCODER_calibration(){
    PIOC->PIO_CODR|= PIO_PC23; //set output to high
    PWM->PWM_CH_NUM[0].PWM_CDTY = 7000; //fornuftig duty cycle

    uint32_t COUNT = 0;
    while(COUNT < 10000000){
        COUNT++;}

    int16_t offset = ENCODER_get_raw_position();
    return  offset;
}

void ENCODER_PWM_setup(){

    PMC->PMC_PCER1 |= (1<<4); //Enable clock for PIOB

    PIOB->PIO_PER |= PIO_PB12; //enable PIO for PB13
    PIOB->PIO_OER |= PIO_PB12; //enable output for PB13
    PIOB->PIO_PDR |= PIO_PDR_P12; // disable PIO for PB13
    PIOB->PIO_ABSR |= PIO_ABSR_P12; // enable peripheral B for PB13
    PWM->PWM_CH_NUM[0].PWM_CMR |= ((0<<PWM_CMR_CPRE_Pos) & PWM_CMR_CPRE_Msk) | PWM_CMR_CPOL; //set clock polarity
    PWM->PWM_CH_NUM[0].PWM_CMR |= PWM_CMR_CPRE_MCK_DIV_128; //set clock prescaler
    //PIOB->PIO_SODR = PIO_PB13; //set output to high

    PWM->PWM_CH_NUM[0].PWM_CPRD |= 13125; //period(20ms)
    //PWM->PWM_CH_NUM[1].PWM_CDTY = 1378; //duty 2.1 ms
    PWM->PWM_CH_NUM[0].PWM_CDTY |= 0; //duty 0 ms

    PWM->PWM_ENA = (1<<0); //enable ch1

}



void ENCODER_set_duty_cycle(double duty_cycle_ms, uint8_t direction){

    if(duty_cycle_ms > 10){
        duty_cycle_ms = 10;
    }if(duty_cycle_ms < 0){
        duty_cycle_ms = 0;
    }
    PWM->PWM_CH_NUM[0].PWM_CPRD = 13125; //period(20ms)
    double mellomledd = 84000.0/128.0;
    double dutyf = duty_cycle_ms*mellomledd; //duty to wanted value
    uint16_t duty;
    duty = (uint16_t)dutyf;

    PWM->PWM_CH_NUM[0].PWM_CDTY = duty; //sets it

   
    if (direction == 1){
        PIOC->PIO_SODR|= PIO_PC23; //set output to high
        
    }
    if (direction == 0){
        PIOC->PIO_CODR|= PIO_PC23; //set output to low
    }
        
}
double ENCODER_joystick_to_duty_cycle(uint8_t joystick_value){
    double duty_cycle_ms;
    double abs_value;
    if(joystick_value < 50){
        abs_value = 100-joystick_value;
    }else{
        abs_value = joystick_value;}
    if(abs_value < 55){
        duty_cycle_ms = 0;
    }else{
    duty_cycle_ms = 0.1*abs_value;
    }
    return duty_cycle_ms;
}

void ENCODER_control_duty_cycle_with_joystick(void){
    
    if (message.id == 4){
        uint8_t joy_pos = message.byte[0];
        double duty_cycle = ENCODER_joystick_to_duty_cycle(joy_pos);
        if (joy_pos > 50){
            ENCODER_set_duty_cycle(duty_cycle,1);
        }
        else if (joy_pos < 50){
            ENCODER_set_duty_cycle(duty_cycle,0);
        }
        else{
            ENCODER_set_duty_cycle(0,0);
        }
    }
    
}

void ENCODER_set_duty_cycle_with_PID(double controller_gain){
    double duty_cycle_ms = abs(controller_gain/100);
    if (controller_gain > 0){
        ENCODER_set_duty_cycle(duty_cycle_ms,1);
    }
    else if (controller_gain < 0){
        ENCODER_set_duty_cycle(duty_cycle_ms,0);
    }
    else{
        ENCODER_set_duty_cycle(0,0);
    }
    
}

void ENCODER_position_PI_regulator(){
    double reference = message.byte[0];
    double position = (double)ENCODER_get_position();
    double error = reference - position;
    double Kp = 50.0;
    double Ki = 0.1;
    double Kd = 7.5;
    double integral = 0.0;
    double derivative = 0.1;
    double dt = interrupt_tid_ms;
    double duty_cycle_ms = 0.0;
    double controller_gain = 0.0;
    double prev_error = 0.0;
    double prev_controller_gain = 0.0;

    if (((message.id) == 4)/*&&PID_RUN_FLAG*/){ //her kan vi også velge å bruke interrupts men vi har ikke tunet PID for det, så det er litt mye oversving osv
        position = ENCODER_get_position();
        error = reference - position;
        integral = integral + error*dt;
        derivative = (error - prev_error)/dt;
        controller_gain = Kp*error + Ki*integral + 0*Kd*derivative;

        ENCODER_set_duty_cycle_with_PID(controller_gain);

        prev_error = error;
        prev_controller_gain = controller_gain;
        int16_t contr_gain = (int16_t)controller_gain;
        printf("antall mal %d\r\n", goals);
    }
    PID_RUN_FLAG = 0;
}

void ENCODER_interrupt_init(uint32_t timer_tid_ms){
    PMC->PMC_PCER0 |= (1<<ID_TC0);
    
    TC0->TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_TIMER_CLOCK4| TC_CMR_WAVSEL_UP_RC; //| TC_CMR_ETRGEDG_RISING | // Set up TC0 channel 0 TC_CMR_TCCLKS_XC0, where X



    
    uint32_t rc_maks_verdi = (84000000/128)* timer_tid_ms/1000;
    TC0->TC_CHANNEL[0].TC_RC = rc_maks_verdi; //høyeste verdi

    TC0->TC_CHANNEL[0].TC_IER = TC_IMR_CPCS; // VELGER INTERRUPT PÅ CPCS
    NVIC_EnableIRQ(TC0_IRQn);
    
    TC0->TC_CHANNEL[0].TC_CCR |= TC_CCR_CLKEN | TC_CCR_SWTRG; // Enable clock and start counter

}

void TC0_Handler(void) {
    if ((TC0->TC_CHANNEL[0].TC_SR & TC_SR_CPCS)!=0) { //HVIS INTERRUPT SKJER
      PID_RUN_FLAG= 1;
      
    }
}