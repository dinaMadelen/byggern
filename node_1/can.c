#include "can.h"
extern Message2 malmelding2;

void CAN_init(void)
{
    SPI_MasterInit();
}

void CAN_loopback_mode(void)
{
    
}
void CAN_send_message(Message* message)
{
    // (message->ID)
    MCP_write_to_mcp(((message->ID) / 8),MCP_TXB0SIDH);
    MCP_write_to_mcp(((message->ID) % 8)<< 5,MCP_TXB0SIDL);

    MCP_write_to_mcp((message->length),MCP_TXB0DLC);

    for (int i = 0; i < (message->length); i++)
    {
        MCP_write_to_mcp((message->data[i]),MCP_TXB0D0 + i);
    };
    MCP_request_to_send(0);
}

void CAN_recieve_message2(void){

    uint8_t SIDL = MCP_read_from_mcp(MCP_RXB0SIDL)/0b100000;
    uint8_t SIDH = MCP_read_from_mcp(MCP_RXB0SIDH);

    malmelding2.ID = SIDL + SIDH*0b1000;

    malmelding2.length = MCP_read_from_mcp(MCP_RXB0DLC);
    malmelding2.data = MCP_read_from_mcp(MCP_RXB0D0);
    

    
}

Message CAN_recieve_message(void){

    Message melding = {};
    uint8_t SIDL = MCP_read_from_mcp(MCP_RXB0SIDL)/0b100000;
    uint8_t SIDH = MCP_read_from_mcp(MCP_RXB0SIDH)*0b1000;

    melding.ID = SIDL + SIDH;

    melding.length = MCP_read_from_mcp(MCP_RXB0DLC);

    for (int i = 0; i < (melding.length); i++)
    {
        melding.data[i] = MCP_read_from_mcp(MCP_RXB0D0 + i);
    };

    return melding;
}

void CAN_print_message(Message received_message)
{
	printf("Melding:\r\n");
	printf("Id: %d \r\n", received_message.ID);
	printf("Lengde: %d \r\n", received_message.length);
	printf("Melding: %s \r\n\r\n", received_message.data);
}

void CAN_bus_timing_init(void)
{
    MCP_write_to_mcp(0b01000001,MCP_CNF1);/*5 - 0 er BRP*/
    MCP_write_to_mcp(0b10111101,MCP_CNF2);/*1 er bltmode*/ /*2 er sample mode, vi gjør 1 gang */
    MCP_write_to_mcp(0b00000001,MCP_CNF3); /*NUMME 1 ER SOF*/ /*NUMMER 2 er wake up filter*/ /* 3 siste er ps2*/
}


void CAN_send_joistick_pos(char x_or_y){
    uint8_t data = slider_and_joystick_output('j',x_or_y);
    //uint8_t y = slider_and_joystick_output('j','y');

    Message message = {};

    if (x_or_y == 'x')
    {
        message.ID = 4;
    }
    if (x_or_y == 'y')
    {
        message.ID = 5;
    }
    
    message.length = 1;
    message.data[0] = data;

    CAN_send_message(&message);

};

void SEND_right_button_press(void){
    if (is_right_button_pressed()){
        Message message = {};
        message.ID = 1;
        message.length = 0;
        message.data[0] = 0;

        CAN_send_message(&message);
    }

}
