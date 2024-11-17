
#include <stdio.h>
#include "sam/sam3x/include/sam.h"

void PWM_setup();
void SERVO_control_duty_cycle_with_joystick();
void SERVO_set_duty_cycle(double duty_cycle_ms);
double SERVO_joystick_to_duty_cycle(uint8_t joystick_value);
