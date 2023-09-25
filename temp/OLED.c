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
	_delay_us(100);
}

void OLED_write_data_line(uint8_t* data){
	volatile char *oled = (char *) 0x1200; // Start address for the ADC
	oled[0] = (char) *data;
	_delay_us(100);
}

void OLED_write_command(uint8_t data){
	volatile char *oled = (char *) 0x1000; // Start address for the data in OLED
	oled[0] = (char) data;
	_delay_us(100);
}


void OLED_reset_cursor(){
	
	OLED_set_cursor(0x00, 127, 0x00, 0x07);
	
	/*
	OLED_write_command(0x21);	// Set collumn address
	OLED_write_command(0x00);	// Reset collumn start address
	OLED_write_command(127);	// Reset collum end address 

	OLED_write_command(0x22);	// Set page address
	OLED_write_command(0x00);	// Reset page start address 
	OLED_write_command(0x07);	// Reset page end address
	*/

}

void OLED_set_cursor(uint8_t collumn_start, uint8_t collumn_end, uint8_t page_start, uint8_t page_end){
	OLED_write_command(0x21);	// Set collum address
	OLED_write_command(collumn_start);	// Set collum start address
	OLED_write_command(collumn_end);	// Set collum end address 

	OLED_write_command(0x22);	// Set page address
	OLED_write_command(page_start);	// Set page start address 
	OLED_write_command(page_end);	// Set page end address
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
	OLED_write_command(0x00);	// horizontal
}

void OLED_write_string(char* data, uint32_t length){
	//OLED_reset_cursor();
	unsigned char byte;
	for (int j = 0; j < length; j ++){
		for (int i = 0; i < 5; i++){
			byte = pgm_read_byte(&(font5[data[j]-32][i]));
			OLED_write_data(byte);
		}
	}
	
	
	/*
	// Exclamation mark
	for(int j=0;j<1;j++){
		OLED_write_data(0b00000000);
		OLED_write_data(0b00000000);
		OLED_write_data(0b01011111);
		OLED_write_data(0b00000000);
		OLED_write_data(0b00000000);
		//printf("%d", font5[data-32]);
	}*/
}


void OLED_menu(){
	OLED_flush();
	OLED_reset_cursor();
	OLED_set_cursor(0x04, 127, 0x00, 0x07);
	OLED_write_string((char*)"Menu", 4);
	
	OLED_set_cursor(0x04, 127, 0x02, 0x07);
	OLED_write_string((char*)"Start game", 10);
	
	OLED_set_cursor(0x04, 127, 0x03, 0x07);
	OLED_write_string((char*)"Settings", 8);
}

/*

void OLED_printf(char* string, uint32_t length){
	fdevopen(OLED_Transmit, OLED_Receive);
}

void OLED_Transmit(unsigned char data){
	OLED_write_string(&data, 1);
}

unsigned char OLED_Receive( void){
	return 0;
}

*/
