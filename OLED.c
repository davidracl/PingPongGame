#include "helpers.h"
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

void OLED_write_string(char* data, uint32_t length, uint8_t font_size){
	//OLED_reset_cursor();
	unsigned char byte;
	for (int j = 0; j < length; j ++){
		for (int i = 0; i < font_size; i++){
			switch (font_size){
				case 8: byte = pgm_read_byte(&(font8[data[j]-32][i]));
				break;
				case 5: byte = pgm_read_byte(&(font5[data[j]-32][i]));
				break;
				default: byte = pgm_read_byte(&(font4[data[j]-32][i]));
				break;
			}
			OLED_write_data(byte);
			
		}
		OLED_write_data(0x00);
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




void OLED_menu(struct ArrowPosition* ArrowPosition){
	OLED_flush();
	OLED_reset_cursor();
	OLED_set_cursor(MenuCollum, MaxCollum, PageMenu, MaxPage);
	OLED_write_string((char*)"Menu", 4, 8);
	
	OLED_set_cursor(MenuCollum, MaxCollum, PageStartGame, MaxPage);
	OLED_write_string((char*)"Start game", 10, 5);
	
	OLED_set_cursor(MenuCollum, MaxCollum, PageSettings, MaxPage);
	OLED_write_string((char*)"Settings", 8, 5);
	
	/*
	ArrowPosition->row = PageStartGame;
	ArrowPosition->collumn = MinCollum;
	OLED_print_arrow(ArrowPosition->collumn, ArrowPosition->row);*/
	
}

volatile void OLED_print_arrow(uint8_t collum, uint8_t row){
	OLED_set_cursor(collum, 127, row, 0x07);
	OLED_write_data (0b00011000);
	OLED_write_data (0b00011000);
	OLED_write_data (0b01111110);
	OLED_write_data (0b00111100);
	OLED_write_data (0b00011000);
}

volatile void OLED_clear_arrow(uint8_t collum, uint8_t row){
	OLED_set_cursor(collum, 127, row, 0x07);
	OLED_write_data (0x00);
	OLED_write_data (0x00);
	OLED_write_data (0x00);
	OLED_write_data (0x00);
	OLED_write_data (0x00);
}



void OLED_set_arrow_line(struct ArrowPosition* ArrowPosition, uint8_t arrow_row){
	//OLED_clear_arrow(ArrowPosition->collumn, ArrowPosition->row);
	OLED_flush();
	OLED_menu(ArrowPosition);
	OLED_print_arrow(ArrowPosition->collumn, arrow_row);
	ArrowPosition->row = arrow_row;
	
}

enum PageEnum OLED_page_selector(struct ArrowPosition* ArrowPosition){
	printf("%u row is selected \r\n", ArrowPosition->row);
	
	return MainPage;
}