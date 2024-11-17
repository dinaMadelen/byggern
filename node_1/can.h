#include "spi.h"
#include "adc.h"
typedef struct  message
{
    uint16_t ID;
    uint8_t length;
    char data[8];
} Message;

typedef struct  message2
{
    uint16_t ID;
    uint8_t length;
    uint8_t data;
} Message2;


Message CAN_recieve_message(void);
void CAN_recieve_message2(void);
void CAN_send_message(Message* message);
void CAN_init(void);
void CAN_print_message(Message received_message);
void CAN_bus_timing_init(void);
void CAN_send_joistick_pos(char x_or_y);
void SEND_right_button_press(void);