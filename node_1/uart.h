#pragma once
#define FOSC 4915200//clock speed
#define BAUD 9600 
#define MYUBRR FOSC/16/BAUD-1

void USART_Init(unsigned int ubrr);
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );