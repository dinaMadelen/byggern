#include <avr/io.h>
#include <stdio.h>
#include "uart.h"
#include <avr/delay.h>
#include <stdlib.h>
#include "sram.h"
#include "adc.h"
#include "oled.h"




void draw_menu(uint8_t menu_type);
void navigate_menu_with_joystick(void);
uint8_t get_menu_type();
void menu_func(void);
