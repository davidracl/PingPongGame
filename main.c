/*
 * FirstTest.c
 *
 * Created: 29.08.2023 13:00:10
 * Author : klevisr
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "helpers.h"  
#include "USART.h"
#include "OLED.h"
#include "ADC.h"
#include "SRAM.c"

struct ArrowPosition* ArrowPosition;
uint8_t InterruptFlag;

int main( void )
{
	USART_Init ( MYUBRR );
	fdevopen(USART_Transmit, USART_Receive);
	initPWM();

	MCUCR |= (1 << SRE);
	
	SFIOR &= ~(111 << XMM0);
	SFIOR |= (1 << XMM2);
	
	EMCUCR |= (1 << SRW01) | (1 << SRW00);
	
	// Setup interups
	DDRD  = 0b11111011;   // set PD2 to input 
	GICR |= (1<<INT0);     // Enable INT0 External Interrupt
	MCUCR |= (1<<ISC01);   // Falling-Edge Triggered INT0
	sei();     // Enable Interrupts	setup_joystick();
	
	SRAM_test();
	setup_joystick();
	
	InterruptFlag = 0;
	
	struct joystickPosition* joystickPosition;
	
	OLED_init();
	OLED_flush();
	OLED_set_horizontal_mode();
	OLED_reset_cursor();
	
	//char data[12] = "Hello world";
	//uint32_t length = sizeof(data)/sizeof(char);
	//OLED_write_string(&data, length);
	
	OLED_menu(ArrowPosition);
	ArrowPosition->row = PageStartGame;
	ArrowPosition->collumn = MinCollum;
	ArrowPositionNumber = PageStartGame;
	OLED_print_arrow(ArrowPosition->collumn, ArrowPosition->row);
	
	uint8_t LocalInterruptFlag;
	

	while(1){
		//adc_read();
		// _delay_ms(100);
		get_joystick_position(joystickPosition);
		//_delay_us(100);
		//printf("%d\r\n", joystickPosition->position);
		// OLED_set_arrow_line(ArrowPosition, PageSettings);

		printf("Row1: %d\r\n", ArrowPositionNumber);
		
		if (joystickPosition->position == 3){ //DOWN
			OLED_set_arrow_line(ArrowPosition, PageSettings);
			printf("Row: %d\r\n", ArrowPosition->row);
		}
		else if (joystickPosition->position == 2){ //UP
			OLED_set_arrow_line(ArrowPosition, PageStartGame);
			printf("Row: %d\r\n", ArrowPosition->row);
		}
		printf("RowO: %d\r\n", ArrowPositionNumber);
		LocalInterruptFlag = InterruptFlag;
		if (LocalInterruptFlag){
			printf("Interrupt: %d \r\n", ArrowPositionNumber);
			OLED_page_selector(ArrowPosition);
			InterruptFlag = 0;
		}
		
	}
	
	
	
	/*
	
	OLED_write_command(0x22);
	OLED_write_command(0x00);
	OLED_write_command(0x00);
	
	OLED_write_command(0x21);
	OLED_write_command(0x00);
	OLED_write_command(0x00);
	
	OLED_write_command(0xA7);
	
	*/
	
	
	/*
	volatile char *adc = (char *) 0x1400; // Start address for the ADC
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
		
	setup_joystick();
	struct adc_output newValues;
	double angle;
	double dist;
	double positionX;
	double positionY;
	
	while (1)
	{
			struct joystickPosition joystickPos = get_joystick_position();
			struct slidersPosition sliderPos = get_sliders_position();
			
			printf("pos: %d, left slider:%d , right slider: %d \r\n", joystickPos.position, sliderPos.left, sliderPos.right);
			 
	}
	
	*/
}


ISR(INT0_vect) 
{
	 GIFR&= ~(1<<INTF0);
	 InterruptFlag = 1;
}
