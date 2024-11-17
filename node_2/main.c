#include <stdio.h>
#include <stdarg.h>
#include "can.h"
#include "uart.h"
#include "time.h"
#include "sam/sam3x/include/sam.h"
#include "servo.h"
#include "adc.h"
#include "encoder.h"
#include "solenoid.h"

#define F_CPU 84000000
uint8_t kolengde = 0;
uint8_t goals = 0;
CanMsg message = {};
CanMsg malmelding;
int16_t OFFSET;
uint16_t COUNTER = 0;
uint8_t PID_RUN_FLAG = 0;
uint16_t interrupt_tid_ms = 100;
uint16_t COUNTER_2 = 0;
uint8_t ADC_row_counter = 0;


int main()
{
    SystemInit();

    uint16_t COUNTER;WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    //Uncomment after including uart above
    uart_init(84000000, 9600);


    can_init_super_viktig_riktig_ikke_touch_for_faen();

  
    ADC_init();
    ENCODER_init();
    PWM_setup();
    SOLENOID_init();
    ENCODER_PWM_setup();
    OFFSET = ENCODER_calibration();

    //ENCODER_interrupt_init(interrupt_tid_ms);
    
    while (1)
    {

        /* code */

        //DENNE TRENGER VI FOR Å TA IMOT MELDINGER!
        can_rx(&message); // IKKE FJERN DETTE!


        
        SERVO_control_duty_cycle_with_joystick();
        //ENCODER_control_duty_cycle_with_joystick();
       
        ENCODER_position_PI_regulator();
        SOLENOID_fire();
        count_goals();
    

        if (goals >= 3){
            for(int i = 0; i <4; i++){
                malmelding.id = 2;
                malmelding.length = 3;
                malmelding.byte[0] = 4;
                can_tx(malmelding);
                }
            goals = 0;
        } 
        
    }
    
}