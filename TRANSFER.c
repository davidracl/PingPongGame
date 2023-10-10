#include "TRANSFER.h"

// Send joystick Position via CAN with ID 100
void transfer_joystick_position(struct joystickPosition* joystickPosition){
	
	struct CAN_Message joystick_message;
	joystick_message.ID = 100;		// Joystick Position: ID 100
	joystick_message.data[0] = (uint8_t)joystickPosition->position;
	joystick_message.length = 1;
	
	can_send(&joystick_message);	// send message
}