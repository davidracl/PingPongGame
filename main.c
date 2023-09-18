/*
 * FirstTest.c
 *
 * Created: 29.08.2023 13:00:10
 * Author : klevisr
 */ 

#include <stdio.h>
#include <stdlib.h>

#include "USART.h"
#include "JOYSTICK.h"
#include "SLIDERS.h"
#include "OLED.h"



void main( void )
{
	USART_Init ( MYUBRR );
	fdevopen(USART_Transmit, USART_Receive);
	initPWM();
	OLED_init();
	
	
		
	MCUCR |= (1 << SRE);
	
	SFIOR &= ~(111 << XMM0);
	SFIOR |= (1 << XMM2);
	
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
	
	
}

