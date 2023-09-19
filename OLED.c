#include "OLED.h"
#include "fonts.h"
void OLED_init()
{
 // display off
 OLED_write_command(0xae);
 // segment remap
 OLED_write_command(0xa1);
 // common pads hardware: alternative
 OLED_write_command(0xda);
 OLED_write_command(0x12);
 // common output scan direction:com63~com0
 OLED_write_command(0xc8);
 // multiplex ration mode:63
 OLED_write_command(0xa8);
 OLED_write_command(0x3f);
 // display divide ratio/osc. freq. mode
 OLED_write_command(0xd5);
 OLED_write_command(0x80);
 // contrast control
 OLED_write_command(0x81);
 OLED_write_command(0x50);
 // set pre-charge period
 OLED_write_command(0xd9);
 OLED_write_command(0x21);
 // Set Horizontal Memory Addressing Mode
 OLED_write_command(0x20);
 OLED_write_command(0x00);

 // start reading at RAM address = 0;
 OLED_write_command(0x40);

 // VCOM deselect level mode
 OLED_write_command(0xdb);
 OLED_write_command(0x30);
 // master configuration
 OLED_write_command(0xad);
 OLED_write_command(0x00);
 // out follows RAM content
 OLED_write_command(0xa4);
 // set normal display
 OLED_write_command(0xa6);
 // display on
 OLED_write_command(0xaf);
}

void OLED_write_data(uint8_t data){
	volatile char *oled = (char *) 0x1200; // Start address for the ADC
	oled[0] = (char) data;
	_delay_ms(100);
}

void OLED_write_command(uint8_t data){
	volatile char *oled = (char *) 0x1000; // Start address for the ADC
	oled[0] = (char) data;
	_delay_ms(100);
}


void OLED_reset_cursor(){
	
	OLED_write_command(0x21);
	OLED_write_command(0x00);
	OLED_write_command(127);

	OLED_write_command(0x22);
	OLED_write_command(0x00);
	OLED_write_command(7);

}

void OLED_flush(){
	 OLED_reset_cursor();
	 //all pixels on 
	for (int i = 0; i < 8 * 128; i++){
			OLED_write_data(0x00);
	}
	OLED_reset_cursor();
}

void OLED_set_horizontal_mode(){
	
	OLED_write_command(0x20);
	OLED_write_command(0x00);
}

void OLED_write_string(char data){
	OLED_reset_cursor();
	
		for(int j=0;j<5;j++)
			OLED_write_data(font5[6][j]);
}
