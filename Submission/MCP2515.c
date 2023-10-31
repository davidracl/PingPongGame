#include "MCP2515.h"

void mcp2515_init()
{
	SPI_init() ; // Initialize SPI
	mcp2515_reset(); // Send reset command
	
	// Self test
	uint8_t value = mcp2515_read(MCP_CANSTAT);
	if ((value & MODE_MASK) != MODE_CONFIG) {
		printf (" MCP2515 is NOT in configuration mode after reset !\r\n");
	}
	
	// Set bit rate
	uint8_t BRP = MCP_FREQ / (2 * 16 * MCP_BAUDRATE);
	
	// Configuration for bit timing (matches Node2)  
	mcp2515_write(SJW4 | (BRP - 1), MCP_CNF1);
	mcp2515_write(BTLMODE | SAMPLE_3X | ((MCP_PHASE1 - 1) << 3) | (MCP_PROPAG - 1), MCP_CNF2);
	mcp2515_write(WAKFIL_DISABLE | (MCP_PHASE2 - 1), MCP_CNF3);
	
}

uint8_t mcp2515_read ( uint8_t address )
{
	uint8_t result ;
	PORTB &= ~(1 << PB4);	// Select CAN controller
	
	SPI_write (MCP_READ);	// Send read instruction
	SPI_write (address);	// Send address
	result = SPI_read();	// Read result
	PORTB |= (1 << PB4);	// Deselect CAN controller
	return result ;
}

void mcp2515_reset(){
	PORTB &= ~(1<<PB4);		// Select CAN controller
	SPI_write(MCP_RESET);	// Send reset instruction
	PORTB |= (1<<PB4);		// Deselect CAN controller
}

void mcp2515_write(uint8_t data, uint8_t address){
	PORTB &= ~(1<<PB4);
	SPI_write(MCP_WRITE);
	SPI_write(address);
	SPI_write(data);
	PORTB |= (1<<PB4);
}

void mcp2515_request_to_send(){
	PORTB &= ~(1<<PB4);
	SPI_write(MCP_RTS_TX0);
	PORTB |= (1<<PB4);
}

void mcp2515_request_to_send_v2(){
	PORTB &= ~(1<<PB4);
	SPI_write(MCP_TXB0CTRL);
	PORTB |= (1<<PB4);
}

void mcp2515_bit_modify(uint8_t data, uint8_t mask, uint8_t address){
	PORTB &= ~(1<<PB4);
	SPI_write(MCP_BITMOD);
	SPI_write(address);
	SPI_write(mask);
	SPI_write(data );
	PORTB |= (1<<PB4);
}

uint8_t mcp2515_read_status(){
	PORTB &= ~(1<<PB4);
	SPI_write(MCP_READ_STATUS);
	PORTB |= (1<<PB4);
	return SPI_read();
}

void mcp2515_set_mode(uint8_t mode) {
	mcp2515_bit_modify(mode, 0b11100000, MCP_CANCTRL);
}