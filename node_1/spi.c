#include "spi.h"



void SPI_MasterInit(void)
{
    /* Set MOSI and SCK output, pluss SS til output så den kan velge CS på CAN, all others input */
    DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);
    /* Enable SPI, Master, set clock rate fck/16, muliggjør interrupts */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPIE);

    SPI_high_ss();

};

void SPI_write(char data)
{
/* Start transmission */
    SPDR = data;
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF))){

    };
}

uint8_t SPI_read(void)
{
/* Wait for reception complete */
    SPDR = 0xFF; //dummy byte
    while(!(SPSR & (1<<SPIF))){

    };
/* Return data register */
return SPDR;
}

void SPI_high_ss(void){
    PORTB |= (1 << DD_SS);
}

void SPI_low_ss(void){
    PORTB &= ~(1 << DD_SS);
}

void MCP_reset(void){
    SPI_low_ss();
    SPI_write(MCP_RESET);
    SPI_high_ss();
}

void MCP_init(void){
    SPI_MasterInit();
    MCP_reset();
    _delay_ms(10);

	// Sjøltesting
	uint8_t value = MCP_read_from_mcp(MCP_CANSTAT); //Se kode for denne lenger ned
	if ((value & MODE_MASK) != MODE_CONFIG) {
		printf("MCP2515 er ikke i konfigurasjonsmodus etter reset. CANSTAT: %x \r\n", value);
	}
}

uint8_t MCP_read_from_mcp(uint8_t address){
    SPI_low_ss(); //må pulles low for å aktivere slave
    SPI_write(MCP_READ); //Setter i lesemodus
    SPI_write(address);
    uint8_t data = SPI_read();
    SPI_high_ss();
    return data;
}

void MCP_write_to_mcp(uint8_t data, uint8_t address){
    SPI_low_ss();

    SPI_write(MCP_WRITE);
    SPI_write(address);
    SPI_write(data);

    SPI_high_ss();
}

void MCP_request_to_send(uint8_t select_buffer)// if 3 then we req to senda all
{
    SPI_low_ss();

    if (select_buffer == 0)
    {
        SPI_write(MCP_RTS_TX0);
    }
    else if(select_buffer == 1)
    {
        SPI_write(MCP_RTS_TX1);
    }
    else if(select_buffer == 2)
    {
        SPI_write(MCP_RTS_TX2);
    }
    else if (select_buffer == 3)
    {
        SPI_write(MCP_RTS_ALL);
    }
    else
    {
        printf("invalid request to send input\r\n");
    };

    SPI_high_ss();
}

uint8_t MCP_read_status(void){
    SPI_low_ss();
    SPI_write(MCP_READ_STATUS);
    uint8_t data = SPI_read();
    SPI_high_ss(); //5318008

    return data;
}

void MCP_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
	SPI_low_ss();
	SPI_write(MCP_BITMOD);
	SPI_write(address); 
	SPI_write(mask); //Maskeringsbyte som forteller hvilke bits vi vil endre
	SPI_write(data); //Verdiene som bitene skal endres til
	SPI_high_ss();
}

void MCP_set_mode(uint8_t mode) {
	MCP_bit_modify(MCP_CANCTRL, 0b11100000, mode);
}


