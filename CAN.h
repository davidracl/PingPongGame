#include "MCP2515.h"

struct CAN_Message{
	uint16_t ID;
	uint8_t* data[8];
	uint32_t length;
	};
	
//struct CAN_Message *can_message;

void CAN_init(uint8_t mode);
void can_send(struct CAN_Message *can_message);
struct CAN_Message* can_receive(uint8_t buffer);
void CAN_generate_message(uint8_t ID, uint8_t data, uint32_t length);