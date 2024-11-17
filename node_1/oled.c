#include "oled.h"
#include <avr/delay.h>
#include "fonts.h"


void write_oled_command(uint8_t command)
{
    volatile char *ext_ram = (char *) 0x1000; // Start address for the SRAM
    uint16_t ext_ram_size = 0x200;

    ext_ram[0] = command;
}

void write_oled_data(uint8_t command)
{
    volatile char *ext_ram = (char *) 0x1200; // Start address for the SRAM
    uint16_t ext_ram_size = 0x200;

    ext_ram[0] = command;
}


void oled_init()
{
    write_oled_command(0xae); // display off
    write_oled_command(0xa1); //segment remap
    write_oled_command(0xda); //common pads hardware: alternative
    write_oled_command(0x12);
    write_oled_command(0xc8); //common output scan direction:com63~com0
    write_oled_command(0xa8); //multiplex ration mode:63
    write_oled_command(0x3f);
    write_oled_command(0xd5); //display divide ratio/osc. freq. mode
    write_oled_command(0x80);
    write_oled_command(0x81); //contrast control
    write_oled_command(0x50);
    write_oled_command(0xd9); //set pre-charge period
    write_oled_command(0x21);
    write_oled_command(0x20);
    write_oled_command(0x02);
    write_oled_command(0xdb); //VCOM deselect level mode
    write_oled_command(0x30);
    write_oled_command(0xad); //master configuration
    write_oled_command(0x00);
    write_oled_command(0xa4); //out follows RAM content
    write_oled_command(0xa6); //set normal display
    write_oled_command(0xaf); // display on

    _delay_ms(100);

    PAGE_ADRESSING_MODE();
}

void PAGE_ADRESSING_MODE()
{
    write_oled_command(0x20);
    write_oled_command(0b10);
}

void HORIZONTAL_ADRESSING_MODE()
{
    write_oled_command(0x20);
    write_oled_command(0b00);
}

void VERTICAL_ADRESSING_MODE()
{
    write_oled_command(0x20);
    write_oled_command(0b01);
}

void oled_goto_line(uint8_t line)
{
    
    uint8_t LINE = 0xB0 + line;
    write_oled_command(LINE);
}

void oled_goto_column(uint8_t column)
{
    
    write_oled_command(0x00 + (column % 16));
    write_oled_command(0x10 + (column / 16));
}


void oled_reset()
{
    oled_init();
    for (int line = 0; line < 8; line++) {
	oled_goto_line(line);
	oled_goto_column(0);
	for (int i = 0; i < 128; i++) {
		write_oled_data(0x00);
	}
    }
};

void oled_print_char(char letter)
{
    letter = letter-32;
    for (int i = 0; i < 8; i++) {

    int byte = pgm_read_byte(&font8[letter][i]);
    write_oled_data(byte);
    }
}

void oled_print(char string[])
{
    for (int i = 0; i < strlen(string); i++) {

        oled_print_char(string[i]);
    }
}