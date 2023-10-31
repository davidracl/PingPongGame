#include "CAN.h"

void CAN_init(uint8_t mode){
	
	// Initialize CAN controller
	mcp2515_init();
	mcp2515_set_mode(mode);
	
	_delay_ms(1);
	
	// Check if in correct mode
	uint8_t value = mcp2515_read(MCP_CANSTAT);
	
	if ((value & MODE_MASK) != mode) {
		printf (" MCP2515 is NOT in set mode %u!\r\n", value);
	}
	
	// Activate Interrupts for receive buffers
	mcp2515_write(0b00000011, MCP_CANINTE);  
	
}



void can_send(struct CAN_Message* can_message) {
	
	// Send message (ID, length and data)
	mcp2515_write(can_message->ID / 8, MCP_TXB0SIDH);
	mcp2515_write((can_message->ID % 8) << 5, MCP_TXB0SIDL); 

	mcp2515_write( can_message->length, MCP_TXB0DLC);	

	for (int i = 0; i < can_message->length; i++) {
		mcp2515_write(can_message->data[i], MCP_TXB0D0 + i );
	}
	
	mcp2515_request_to_send();
}

struct CAN_Message* can_receive(uint8_t buffer){		// Buffer number (0 or 1)
	
	static struct CAN_Message received_message;
	
	// Buffer 0
	if (!buffer){
		_delay_us(1);
		received_message.ID = mcp2515_read(MCP_RXB0SIDH) << 3; 
		received_message.ID += mcp2515_read(MCP_RXB0SIDL) >> 5; 
		received_message.length = mcp2515_read(MCP_RXB0DLC);
	
		uint8_t first_byte = mcp2515_read(MCP_RXB0D0);
		for (int i = 0; i < received_message.length; i++){
			received_message.data[i] = mcp2515_read(MCP_RXB0D0 + i);
		}
	}
	
	// Buffer 1
	else{
		received_message.ID = mcp2515_read(MCP_RXB1SIDH) << 3; 
		received_message.ID += mcp2515_read(MCP_RXB1SIDL) >> 5; 
		received_message.length = mcp2515_read(MCP_RXB1DLC);
	
		uint8_t first_byte = mcp2515_read(MCP_RXB1D0);
		for (int i = 0; i < received_message.length; i++){
			received_message.data[i] = mcp2515_read(MCP_RXB1D0 + i);
		}
	}
	
	return &received_message;
}

