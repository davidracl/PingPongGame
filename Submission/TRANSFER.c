#include "TRANSFER.h"

// Send joystick Position via CAN with ID 100
void transfer_joystick_position(struct joystickPosition* joystickPosition){
	
	struct CAN_Message joystick_message;
	joystick_message.ID = 100;		// Joystick Position: ID 100
	joystick_message.data[0] = (uint8_t)joystickPosition->position;
	joystick_message.length = 1;
	
	can_send(&joystick_message);	// send message
}

// Send touch button pressed via CAN with ID 99
void transfer_touch_button(){
	
	struct CAN_Message touch_button;
	touch_button.ID = 99;		// Touch button: ID 99
	touch_button.data[0] = 1;
	touch_button.length = 1;
	
	can_send(&touch_button);	// send message
}