/*
 * FirstTest.c
 *
 * Created: 29.08.2023 13:00:10
 * Author : klevisr
 */ 

#include <stdio.h>
#include <stdlib.h>

#include "USART.h"
//#include "JOYSTICK.h"
//#include "SLIDERS.h"
#include "OLED.h"
#include "ADC.h"
#include "SRAM.c"




int main( void )
{
	USART_Init ( MYUBRR );
	fdevopen(USART_Transmit, USART_Receive);
	initPWM();
	
	
	MCUCR |= (1 << SRE);
	
	SFIOR &= ~(111 << XMM0);
	SFIOR |= (1 << XMM2);
	
	EMCUCR |= (1 << SRW01) | (1 << SRW00);
	SRAM_test();
	
	OLED_init();
	OLED_flush();
	OLED_set_horizontal_mode();
	OLED_reset_cursor();
	
	
	//#define OLED_CMD_INVERT 0xA6

	
	//volatile char *oled_command = (char *) 0x1000; // Start address for the ADC
	
	
	char data[12] = "Hello world";
	uint32_t length = sizeof(data)/sizeof(char);
	OLED_write_string(&data, length);
	OLED_menu();
	

	while(1){
		_delay_ms(20);
		
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

