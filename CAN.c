#include "CAN.h"

void CAN_init(){

	mcp2515_init();
	mcp2515_set_mode(MODE_LOOPBACK);
	
	_delay_ms(1);

	uint8_t value = mcp2515_read(MCP_CANSTAT);
	
	//value = mcp2515_read(MCP_CANSTAT);
	if ((value & MODE_MASK) != MODE_LOOPBACK) {
		printf (" MCP2515 is NOT in loopback mode %u!\r\n", value);
	}
	
}

void CAN_generate_message(uint8_t ID, uint8_t* data, uint32_t length){
	can_message->data = data;
	can_message->ID = ID;
	can_message->length = length;	
}

void can_send() {
	
	
	
	mcp2515_write(can_message->ID / 8, MCP_TXB0SIDH); 
	mcp2515_write((can_message->ID % 8) << 5, MCP_TXB0SIDL ); 

	mcp2515_write( can_message->length, MCP_TXB0DLC);

	for (int i = 0; i < can_message->length; i++) {
		mcp2515_write(can_message->data[i], MCP_TXB0D0 + i );
	}
	
	mcp2515_request_to_send(0);

}

void can_receive(){
	
	
	can_message->ID = mcp2515_read(MCP_RXB0SIDL)*0b1000; 
	can_message->ID += mcp2515_read(MCP_RXB0SIDH); 


	can_message->length = mcp2515_read(MCP_RXB0DLC);
	
	can_message->data[0] = mcp2515_read(MCP_RXB0D0);
	printf("character:%x \r\n",can_message->data[0]);

/*
	for (int i = 0; i < can_message->length; i++) {
		can_message->data[i] = mcp2515_read(MCP_RXB0D0 + i);
		printf("character:%x \r\n",can_message->data[i]);
	}*/
	
	
}

