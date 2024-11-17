#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>

void ADC_init();
void ADC_test(void);
uint16_t slider_and_joystick_output(char slider_or_joystick, char left_right_x_y);
char direction_of_joystick_DULRN(void);
int is_joystick_button_pressed(void);
int is_right_button_pressed(void);
int is_left_button_pressed(void);