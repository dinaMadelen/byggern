#include <avr/io.h>
#include <stdio.h>
#include "uart.h"
#include <avr/delay.h>
#include <stdlib.h>
#include "mcp2515.h"

// Send-buffer 0
#define MCP_Send0 0x31 // aka 0b0011 0001

//Send-buffer 1
#define MCP_Send1 0x41  //0100 0001

//Send-buffer 2
#define MCP_Send2 0x51 //0101 0001

// Motta-buffer 0
#define MCP_Read0 0x61

// Motta-buffer 1
#define MCP_Read1 0x71

#define DDR_SPI DDRB
#define DD_SS PB4
#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7

void SPI_MasterInit(void);
void SPI_write(char data);
uint8_t SPI_read(void);
void SPI_high_ss(void);
void SPI_low_ss(void);
void MCP_reset(void);
void MCP_init(void);

uint8_t MCP_read_from_mcp(uint8_t address);

void MCP_write_to_mcp(uint8_t data, uint8_t address);

void MCP_request_to_send(uint8_t select_buffer);// if 3 then we req to senda all

uint8_t MCP_read_status(void);

void MCP_bit_modify(uint8_t address, uint8_t mask, uint8_t data);

void MCP_set_mode(uint8_t mode);