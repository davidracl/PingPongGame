/*
 * FirstTest.c
 *
 * Created: 29.08.2023 13:00:10
 * Author : klevisr
 */ 


#include "USART.h"
#include "OLED.h"
#include "ADC.h"
#include "SUPPORT.h"
#include "MCP2515.h"
#include "CAN.h"


uint8_t InterruptFlag;

int main( void )
{
	USART_Init ( MYUBRR );
	PWM_init();
	SRAM_init();
	INTERRUPT_init();
	//SPI_init();
	//mcp2515_init();
	CAN_init();
	
	
	SRAM_test();
	setup_joystick();
	OLED_init();
	OLED_menu_init();	
	
	InterruptFlag = 0;
	struct joystickPosition* joystickPosition;
	static uint8_t data = 0x12;
	uint8_t data2;
	while(1){
		/*
		mcp2515_write(MCP_TXB0SIDH, 0x07); // Skriver 0xA7 til sende-buffer nr. 0
		mcp2515_request_to_send(); // Sender 0xA7 fra bufferen ut på CAN-bussen
		uint8_t byte = mcp2515_read(MCP_RXB0SIDH); // Leser fra mottaksbuffer nr. 0
		printf("mottar: %x\r\n", byte); //Skal være samme som man sender, altså 0xA7
		*/
		/*
		CAN_generate_message(0x00,&data , 1);
		can_send();*/
	
		//can_receive();

		get_joystick_position(joystickPosition);
		OLED_operate_menu(joystickPosition->position);
		
		if (InterruptFlag){
			printf("Interrupt: %d \r\n", ArrowPositionNumber);
			OLED_page_selector();
			OLED_menu_select_element();
			InterruptFlag = 0;
		}
		_delay_us(10);
	}
	
}


ISR(INT0_vect) 
{
	 GIFR&= ~(1<<INTF0);
	 InterruptFlag = 1;
}

ISR(INT1_vect)
{
	printf("Interrupt SPI\r\n");
	GIFR&= ~(1<<INTF0);
	
}
