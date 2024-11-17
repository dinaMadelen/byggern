#include <avr/io.h>
#include <stdio.h>
#include "uart.h"
#include <avr/delay.h>
#include <stdlib.h>
#include "sram.h"
#include "adc.h"
#include "oled.h"
#include "menu.h"
#include "spi.h"
#include "can.h"

uint8_t menu_position[] = {1,1};
uint8_t menu_type = 11;
Message malmelding2 = {1,1,1};



#define FOSC 4915200 //clock speed
#define F_CPU 4915200 // clock speed, sikkert unødvendig med begge, men studass la til når jeg knota med ny fil.
#define BAUD 9600 
#define MYUBRR FOSC/16/BAUD-1

//PUTTY CHECK:  putty -serial /dev/ttyS0 -sercfg 9600,8,n,2,N

int main () {

    USART_Init(MYUBRR);
    SRAM_init();
    ADC_init();
    oled_init();
    MCP_init();
    CAN_init(); // Denne initierer mcp, som initierer spi.
    CAN_bus_timing_init();
	MCP_set_mode(MODE_NORMAL);
    uint16_t PROGRAM_COUNTER = 0;
    oled_reset();
    draw_menu(11);
    
    uint8_t LEAVE_GAME = 0;
    uint8_t ENTER_GAME = 0;

    while(1){

        menu_func();
        uint8_t current_menu_type = get_menu_type(); 
        if(current_menu_type == 12){
                draw_menu(current_menu_type);
                ENTER_GAME = 1;
                LEAVE_GAME = 0;
            }
        if(ENTER_GAME){
            ENTER_GAME = 0;
            while(!LEAVE_GAME){
                if ((PROGRAM_COUNTER%2730) == 0){
                    //printf("Sending message\n\r");
                    CAN_send_joistick_pos('y');
                }
                if ((PROGRAM_COUNTER%2048) == 0){
                    //printf("Sending message\n\r");
                    CAN_send_joistick_pos('x');
                }
                if ((PROGRAM_COUNTER%3088) == 0){
                    SEND_right_button_press();
                    //printf("%d\r\n", MCP_read_status());
                }

                
                if ((PROGRAM_COUNTER%(3088+2048)==0)){
                    uint8_t read_status =MCP_read_status();
                    //printf("STATUS: %d\r\n",read_status);
                    if(read_status > 12){
                        menu_position [0] = 1;
                        menu_position [1] = 3;
                        current_menu_type = get_menu_type();
                        draw_menu(current_menu_type);
                        while(!is_left_button_pressed()){
                            if(is_left_button_pressed()){
                                break;
                            }
                        }
                        menu_position [0] = 1;
                        menu_position [1] = 1;
                        current_menu_type = get_menu_type();
                        draw_menu(current_menu_type);
                        ENTER_GAME = 0;
                        LEAVE_GAME = 1;
                        
                    }
                    
                }
                PROGRAM_COUNTER++;
                
            }
            LEAVE_GAME = 1;
        }
            

            
        
        
    }
    
    
    return 0;
}