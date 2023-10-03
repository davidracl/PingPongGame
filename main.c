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
#include "SPI.h"


uint8_t InterruptFlag;

int main( void )
{
	USART_Init ( MYUBRR );
	PWM_init();
	SRAM_init();
	INTERRUPT_init();
	SPI_init();
	SRAM_test();
	setup_joystick();
	OLED_init();
	OLED_menu_init();	
	
	InterruptFlag = 0;
	struct joystickPosition* joystickPosition;
	
	while(1){
		
		SPI_MasterTransmit(0x04);

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
