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

// Write a byte of data
void OLED_write_data(uint8_t data){
	volatile char *oled = (char *) 0x1200; // Start address for the data section of display
	oled[0] = (char) data;
	_delay_us(100);
}

// Set command
void OLED_write_command(uint8_t command){
	volatile char *oled = (char *) 0x1000; // Start address for the command section of display
	oled[0] = (char) command;
	_delay_us(100);
}


void OLED_reset_cursor(){
	OLED_set_cursor(0x00, 127, 0x00, 0x07);	// Reset cursor to collumn 0 and page 0
}


void OLED_set_cursor(uint8_t collumn_start, uint8_t collumn_end, uint8_t page_start, uint8_t page_end){
	OLED_write_command(0x21);			// Set collum address
	OLED_write_command(collumn_start);	// Set collum start address
	OLED_write_command(collumn_end);	// Set collum end address 

	OLED_write_command(0x22);		// Set page address
	OLED_write_command(page_start);	// Set page start address 
	OLED_write_command(page_end);	// Set page end address
}

// Turn all pixels of the display on or off
void OLED_flush(uint8_t on){
	 OLED_reset_cursor();
	 uint8_t data = (on) ? PixelBlockOn : PixelBlockOff;
	for (int i = 0; i < 8 * 128; i++){
		OLED_write_data(data);
	}
	OLED_reset_cursor();
}


void OLED_set_horizontal_mode(){
	OLED_write_command(0x20);	// Set mode
	OLED_write_command(0x00);	// horizontal
}

// Write a string of characters of length "length" to the OLED
void OLED_write_string(char* data, uint32_t length, uint8_t font_size){
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
		OLED_write_data(0x00); // Space between characters
	}
}


void OLED_menu(){
	OLED_flush(0);			// Turn all pixels off
	OLED_reset_cursor();

	OLED_set_cursor(MenuCollum, MaxCollum, PageMenu, MaxPage);
	OLED_write_string((char*)"Menu", 4, 8);
	
	OLED_set_cursor(MenuCollum, MaxCollum, PageStartGame, MaxPage);
	OLED_write_string((char*)"Start game", 10, 5);
	
	OLED_set_cursor(MenuCollum, MaxCollum, PageSettings, MaxPage);
	OLED_write_string((char*)"Settings", 8, 5);
	
}

void OLED_menu_v2(struct MenuElement* MenuELement){
	OLED_flush(0);			// Turn all pixels off
	OLED_reset_cursor();
	
	printf("Here");
	OLED_set_cursor(MenuCollum, MaxCollum, PageHeadline, MaxPage);
	OLED_write_string(MenuELement->name, MenuELement->lengthOfName, 8);
	
	OLED_set_cursor(MenuCollum, MaxCollum, PageFirstElement, MaxPage);
	OLED_write_string(MenuELement->elements[0]->name, MenuELement->elements[0]->lengthOfName, 5);
	
	OLED_set_cursor(MenuCollum, MaxCollum, PageSecondElement, MaxPage);
	OLED_write_string(MenuELement->elements[1]->name, MenuELement->elements[1]->lengthOfName, 5);

	OLED_set_cursor(MenuCollum, MaxCollum, PageThirdElement, MaxPage);
	OLED_write_string(MenuELement->elements[2]->name, MenuELement->elements[2]->lengthOfName, 5);
}

struct MenuElement* OLED_menu_init(){
	printf("2");
	struct MenuElement Menu = {NULL, "Menu", 4, {NULL, NULL, NULL}, NULL};
	struct MenuElement Empty = {NULL, "", 0, {NULL, NULL, NULL}, NULL};
	struct MenuElement StartGame = {PageFirstElement, "Start Game", 10, {NULL, NULL, NULL}, &Menu};
	struct MenuElement Settings = {PageSecondElement, "Settings", 8, {NULL, NULL, NULL}, &Menu};
	Menu.elements[0] = &StartGame;
	Menu.elements[1] = &Settings;
	Menu.elements[2] = &Empty;
	OLED_menu_v2(&Menu);
	return &Menu;
}

struct MenuElement* OLED_menu_select_element(struct MenuElement* MenuOrigin, uint8_t row){
	OLED_menu_v2(MenuOrigin->elements[row/2-1]);
}

void OLED_print_arrow(uint8_t collum, uint8_t row){
	printf("%d, %d", collum, row);
	OLED_set_cursor(collum, 127, row, 0x07);
	OLED_write_data (0b00011000);
	OLED_write_data (0b00011000);
	OLED_write_data (0b01111110);
	OLED_write_data (0b00111100);
	OLED_write_data (0b00011000);
}

void OLED_clear_arrow(uint8_t collum, uint8_t row){
	printf("Clear arrow");
	OLED_set_cursor(collum, 127, row, 0x07);
	OLED_write_data (0x00);
	OLED_write_data (0x00);
	OLED_write_data (0x00);
	OLED_write_data (0x00);
	OLED_write_data (0x00);
}

void OLED_initialize_arrow_position(){
	
	ArrowPositionNumber = PageFirstElement;
	OLED_print_arrow(MinCollum, ArrowPositionNumber);
}

void OLED_set_arrow_line (uint8_t arrow_row){
	
	printf("Move arrow");
	
	OLED_clear_arrow(MinCollum, ArrowPositionNumber);
	OLED_print_arrow(MinCollum, arrow_row);
	ArrowPositionNumber = arrow_row;
}

void OLED_page_selector(){
	printf("%u row is selected \r\n", ArrowPositionNumber);
}