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


uint8_t InterruptFlag0;		// Interrupt Joystick
uint8_t InterruptFlag1;		// Interrupt CAN
uint8_t InterruptFlag2;		// Interrupt left push button

// Interrupt on receive buffer 0 and 1
uint8_t int_rxb0;
uint8_t int_rxb1;

struct CAN_Message* received_message;

int main( void )
{
	// Initialize everything
	USART_Init ( MYUBRR );
	PWM_init();
	SRAM_init();
	INTERRUPT_init();
	CAN_init(MODE_NORMAL);
	SRAM_test();
	setup_joystick();
	OLED_init();
	OLED_menu_init();	
	
	// Initialize interrupts
	InterruptFlag0 = 0;
	InterruptFlag1 = 0;
	InterruptFlag2 = 0;
	
	struct joystickPosition* joystickPosition;
	
	// Example CAN message
	//static struct CAN_Message message = {32, {0x07, 0xA2, 0x65, 0x91, 0xE4}, 5};
	
	while(1){
		
		
		
		// Test SPI write and read 
		/*
		mcp2515_write(0x09, MCP_TXB0SIDH);		// first data then address!!!
		mcp2515_request_to_send(); 
		uint8_t byte = mcp2515_read(MCP_RXB0SIDH); 
		printf("Byte: %x\r\n", byte); 
		*/
		

		// Interrupt CAN receive message
		if (InterruptFlag1){
			InterruptFlag1 = 0;
			if (int_rxb0){
				_delay_us(100);
				received_message = can_receive(0);
				printf(" Received CAN message \r\nID: %d, Data: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x \r\n", received_message->ID, received_message->data[0], received_message->data[1], received_message->data[2], received_message->data[3], received_message->data[4]);
			}
	
			if (int_rxb1){
				_delay_us(100);
				received_message = can_receive(1);
				printf(" Received CAN message \r\nID: %d, Data: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x \r\n", received_message->ID, received_message->data[0], received_message->data[1], received_message->data[2], received_message->data[3], received_message->data[4]);
			}
		}
		
		// Interrupt Joystick button
		if (InterruptFlag0){
			printf("Interrupt: %d \r\n", ArrowPositionNumber);
			OLED_page_selector();
			OLED_menu_select_element();
			InterruptFlag0 = 0;
		}
		
		// Interrupt touch button Left
		if (InterruptFlag2){
			printf("Interrupt: Left button pushed \r\n");
			transfer_touch_button();
			InterruptFlag2 = 0;
		}
		
		// CAN send and receive message
		/*
		can_send(&message);
		received_message = can_receive();
		printf("ID: %d, Data: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x \r\n", received_message->ID, received_message->data[0], received_message->data[1], received_message->data[2], received_message->data[3], received_message->data[4]);
		*/
		
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
	 GIFR&= ~(1<<INTF0);
	 InterruptFlag0 = 1;
}

// Push button Interrupt
ISR(INT2_vect)
{
	GIFR&= ~(1<<INTF2);
	InterruptFlag2 = 1;
}

// CAN Interrupt
ISR(INT1_vect)
{
	
	GIFR&= ~(1<<INTF1);
	int_rxb0 = mcp2515_read(MCP_CANINTF) & 0x01;
	int_rxb1 = mcp2515_read(MCP_CANINTF) & 0x02;
	mcp2515_write(0x00, MCP_CANINTF);  // Activate Interrupts for receive buffers
	
	InterruptFlag1 = 1;
	printf("Interrupt SPI\r\n");
	
	// Set acceptance filters regarding identifier
	// - RXBxCTRL.FILHIT<2:0> with RXFnSIDH, RXMnSIDH....
}
