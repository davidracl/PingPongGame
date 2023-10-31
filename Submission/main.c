/*
 * FirstTest.c
 *
 * Created: 29.08.2023 13:00:10
 * Author : klevisr
 */ 


#include "USART.h"
#include "SUPPORT.h"
#include "TRANSFER.h"
#include "OLED.h"


uint8_t InterruptJoystick;		
uint8_t InterruptCAN;	
uint8_t InterruptLeftPushButton;		

// Interrupt on CAN receive buffer 0 and 1
uint8_t int_rxb0;
uint8_t int_rxb1;

struct CAN_Message* received_message;

int main( void )
{
	// Initialize everything
	USART_Init(BAUDRATE_FOR_USART);
	PWM_init();
	SRAM_init();
	INTERRUPT_init();
	CAN_init(MODE_NORMAL);
	SRAM_test();
	setup_joystick();
	OLED_init();
	OLED_menu_init();	
	
	// Initialize Interrupts
	InterruptJoystick = 0;
	InterruptCAN = 0;
	InterruptLeftPushButton = 0;
	
	struct joystickPosition* joystickPosition;
	
	while(1){
		
		// Interrupt CAN receive message
		if (InterruptCAN){
			InterruptCAN = 0;
			if (int_rxb0){
				_delay_us(100);
				received_message = can_receive(0);
				if (received_message->ID == 98){
					OLED_print_score(received_message->data[0]);
				}
				printf(" Received CAN message \r\nID: %d, Data: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x \r\n", received_message->ID, received_message->data[0], received_message->data[1], received_message->data[2], received_message->data[3], received_message->data[4]);
			}
	
			if (int_rxb1){
				_delay_us(100);
				received_message = can_receive(1);
				if (received_message->ID == 98){
					OLED_print_score(received_message->data[0]);
				}
				printf(" Received CAN message \r\nID: %d, Data: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x \r\n", received_message->ID, received_message->data[0], received_message->data[1], received_message->data[2], received_message->data[3], received_message->data[4]);
			}
		}
		
		// Interrupt Joystick button
		if (InterruptJoystick){
			printf("Interrupt: %d \r\n", ArrowPositionNumber);
			OLED_menu_select_element();		// Go to sub-menu selected by arrow
			InterruptJoystick = 0;
		}
		
		// Interrupt touch button Left
		if (InterruptLeftPushButton){
			transfer_touch_button();		// Transfer a CAN message for the button
			InterruptLeftPushButton = 0;
		}
		
		// Navigate Menu
		get_joystick_position(joystickPosition);
		OLED_operate_menu(joystickPosition->position);
		
		// Transfer Joystick Position
		transfer_joystick_position(joystickPosition);
		_delay_ms(10);
		
	}
	
}

// Joystick Interrupt
ISR(INT0_vect) 
{
	 GIFR&= ~(1<<INTF0);		// Clear Interrupt Flag
	 InterruptJoystick = 1;
}

// Push button Interrupt
ISR(INT2_vect)
{
	GIFR&= ~(1<<INTF2);			// Clear Interrupt Flag
	InterruptLeftPushButton = 1;
}

// CAN Interrupt
ISR(INT1_vect)
{
	
	GIFR&= ~(1<<INTF1);			// Clear Interrupt Flag
	int_rxb0 = mcp2515_read(MCP_CANINTF) & 0x01;	// receive buffer 0
	int_rxb1 = mcp2515_read(MCP_CANINTF) & 0x02;	// receive buffer 1
	mcp2515_write(0x00, MCP_CANINTF);				// Activate Interrupts for receive buffers
	
	InterruptCAN = 1;
}
