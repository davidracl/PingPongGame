#include "MCP2515.h"

struct CAN_Message{
	uint8_t ID;
	uint8_t* data;
	uint32_t length;
	};
	
struct CAN_Message *can_message;
void CAN_init();
void can_send();
void can_receive();
void CAN_generate_message(uint8_t ID, uint8_t* data, uint32_t length);