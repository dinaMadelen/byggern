#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>

void write_oled_command(uint8_t command);
void write_oled_data(uint8_t command);
void oled_init();

void PAGE_ADRESSING_MODE();
void HORIZONTAL_ADRESSING_MODE();
void VERTICAL_ADRESSING_MODE();

void oled_goto_line(uint8_t line); 
void oled_goto_column(uint8_t column);
void oled_pos(uint8_t line,uint8_t column);

void oled_print_char(char letter);
void oled_print(char string[]);
void oled_reset(); 
void oled_home(); 

