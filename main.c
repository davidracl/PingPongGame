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

void SRAM_test(void)
    {
		volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
        uint16_t ext_ram_size = 0x800;
        uint16_t write_errors = 0;
        uint16_t retrieval_errors = 0;
        printf("Starting SRAM test...\n");
        // rand() stores some internal state, so calling this function in a loop will
        // yield different seeds each time (unless srand() is called before this function)
        uint16_t seed = rand();
        // Write phase: Immediately check that the correct value was stored
        srand(seed);
        for (uint16_t i = 0; i < ext_ram_size; i++) {
            uint8_t some_value = rand();
            ext_ram[i] = some_value;
            uint8_t retreived_value = ext_ram[i];
            if (retreived_value != some_value) {
                printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
                write_errors++;
            }
        }
        // Retrieval phase: Check that no values were changed during or after the write phase
        srand(seed);
        // reset the PRNG to the state it had before the write phase
        for (uint16_t i = 0; i < ext_ram_size; i++) {
            uint8_t some_value = rand();
            uint8_t retreived_value = ext_ram[i];
            if (retreived_value != some_value) {
                printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
                retrieval_errors++;
            }
        }
        printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}

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
	
	#define OLED_CMD_INVERT 0xA6

	
	volatile char *oled_command = (char *) 0x1000; // Start address for the ADC
	char data = "H";
	_delay_us(100);
	 
		OLED_set_horizontal_mode();
		while(1){
			
		OLED_write_string(data);
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

