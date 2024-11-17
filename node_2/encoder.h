#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"
#include "servo.h"
#include "can.h"

void ENCODER_init(void);
int16_t ENCODER_get_raw_position(void);
int16_t ENCODER_get_position(void);
int16_t ENCODER_calibration();
void ENCODER_PWM_setup();
void ENCODER_set_duty_cycle(double duty_cycle_ms, uint8_t direction);
double ENCODER_joystick_to_duty_cycle(uint8_t joystick_value);
void ENCODER_control_duty_cycle_with_joystick(void);
void ENCODER_position_PI_regulator(void);
void ENCODER_interrupt_init(uint32_t timer_tid_ms);
