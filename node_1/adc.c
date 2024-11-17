#include "adc.h"
#include <avr/delay.h>

void ADC_init() {
  MCUCR |= (1 << SRE); // Enable SRAM i ATmega162
  SFIOR |= (1 << XMM2); // Maskerer PC4-PC7 på ATmega162.
  DDRD |= (1 << PD4);
  
  TCCR3A |= (1 << WGM30) | (1 << WGM31);
  TCCR3B |= (1 << WGM32) | (1 << WGM33) | (1 << CS30);
  TCCR3A |= (0 << COM3A1) | (1 << COM3A0);
  OCR3A = 0;

  volatile char *adc = (char *) 0x1400; // Start address for the ADC
  uint16_t ext_ram_size = 0x400;

  adc[0] = 0x04;
    
  _delay_ms(100);

}

void ADC_test(void)
{
    volatile char *adc = (char *) 0x1400; // Start address for the ADC
      adc[0] = 0x04;
    
      _delay_ms(100);

    volatile uint8_t value1 = adc[0]; // Leser 8-bit data fra ADC
    volatile uint8_t value2 = adc[0];
    volatile uint8_t value3 = adc[0];
    volatile uint8_t value4 = adc[0];

    printf("ADC-verdi: %02X\r\n", value3);
    printf("ADC-verdi: %02X\r\n", value4);
    
    _delay_ms(100);
    
    
};

uint16_t slider_and_joystick_output(char slider_or_joystick, char left_right_x_y)
{
    volatile char *adc = (char *) 0x1400; // Start address for the ADC

    adc[0] = 0x04;

    _delay_ms(100);

    volatile uint8_t slider_right = adc[0]; // Leser 8-bit data fra ADC
    volatile uint8_t slider_left = adc[0];
    volatile uint8_t joystick_1 = adc[0];
    volatile uint8_t joystick_2 = adc[0];

    volatile float value = 0;

    switch(slider_or_joystick)
    {
      case 's':
        switch (left_right_x_y)
        {
        case 'l':
          value = slider_left;
          break;
        
        case 'r':
          value = slider_right;
        default:
          break;
        }
        break;

      case 'j':
        switch (left_right_x_y)
        {
        case 'y':
          value = joystick_1;
          break;
        
        case 'x':
          value = joystick_2;
          break;

        default:
          value = joystick_2;
          break;
        }
        break;

      default:
        break;
    }
    value = (float) value;

    if (value > 160)
    {
      value = (50.0f/95.0f)*value - 34.2f;
    }
    else{
      value =(5.0f/16.0f)*value;
    };

    value = (uint16_t) value;


    return value;
}

char direction_of_joystick_DULRN(void){
  volatile uint16_t joystick_x = slider_and_joystick_output('j','x');
  volatile uint16_t joystick_y = slider_and_joystick_output('j','y');


  if  ((40 < joystick_x)&& (joystick_x < 60) && (40 < joystick_y)&&(joystick_y < 60)){
    return 'N';
  }else if ((40 <= joystick_x) && (joystick_x <= 60) && (0 <= joystick_y)&&(joystick_y <= 40)){
    return 'D';
  } else if  ((40 <= joystick_x)&&(joystick_x <= 60) && (60 <= joystick_y)&&(joystick_y <= 100)){
    return 'U';
  } else if  ((0 <= joystick_x)&&(joystick_x <= 40) && (40 <= joystick_y)&&(joystick_y <= 60)){
    return 'L';
  } else if  ((60 <= joystick_x)&&(joystick_x <= 100) && (40 <= joystick_y)&&(joystick_y <= 60)){
    return 'R';
  } else {
    return 'X';
  };

}

int is_joystick_button_pressed(void)
{

  DDRB &= ~(1 << DDB2);
  PORTB |= (1 << PB2);

  volatile char *BPORT = (char *) 0x36;

  volatile uint8_t button_pressed;
  if (button_pressed = (PINB &(1 <<PB2))){
    return 0;
  }else{
    return 1;
  }
}

int is_left_button_pressed(void)
{
  DDRB &= ~(1 << DDB1);
  PORTB |= (1 << PB1);

  volatile char *BPORT = (char *) 0x36;

  volatile uint8_t button_pressed;
  if (button_pressed = (PINB &(1 <<PB1))){
    return 1;
  }else{
    return 0;
  }
}

int is_right_button_pressed(void)
{
  DDRB &= ~(1 << DDB0);
  PORTB |= (1 << PB0);

  volatile char *BPORT = (char *) 0x36;

  volatile uint8_t button_pressed;
  if (button_pressed = (PINB &(1 <<PB0))){
    return 1;
  }else{
    return 0;
  }
}

