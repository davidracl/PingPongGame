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
 
 OLED_set_horizontal_mode();
 OLED_reset_cursor();
 
}

void OLED_set_horizontal_mode(){
	OLED_write_command(0x20);	// Set mode
	OLED_write_command(0x00);	// horizontal
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

// Set command
void OLED_write_command(uint8_t command){
	volatile char *oled = (char *) 0x1000; // Start address for the command section of display
	oled[0] = (char) command;
	_delay_us(100);
}

// Write a byte of data
void OLED_write_data(uint8_t data){
	volatile char *oled = (char *) 0x1200; // Start address for the data section of display
	oled[0] = (char) data;
	_delay_us(100);
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

// Turn all pixels of the display on or off
void OLED_flush(uint8_t on){
	OLED_reset_cursor();
	uint8_t data = (on) ? PixelBlockOn : PixelBlockOff;
	for (int i = 0; i < 8 * 128; i++){
		OLED_write_data(data);
	}
	OLED_reset_cursor();
}

// Write Elements of SelectedMenu to OLED
void OLED_menu(){
	OLED_flush(0);			// Turn all pixels off
	OLED_reset_cursor();
	
	// Write Headline
	OLED_set_cursor(MenuCollum, MaxCollum, PageHeadline, MaxPage);
	OLED_write_string(SelectedMenu->name, SelectedMenu->lengthOfName, 8);
	
	// Write names of elements
	for (int i = 0; i < SelectedMenu->numberElements; i++){
		OLED_set_cursor(MenuCollum, MaxCollum, PageFirstElement + i*MenuElementPageOffset, MaxPage);
		OLED_write_string(SelectedMenu->elements[i]->name, SelectedMenu->elements[i]->lengthOfName, 5);
	}
	
	// Set arrow to first line
	if (SelectedMenu->numberElements > 0) OLED_initialize_arrow_position();
	
	if (!strcmp(SelectedMenu->name, "Start Game")){
		GameStarted = 1;
	}
	else{
		GameStarted = 0;
	}
	
}

// Initialize OLED menu
void OLED_menu_init(){
	
	static struct MenuElement Menu = {NULL, "Menu", 4, 2, {NULL, NULL, NULL}, NULL};
	
	// Return Element (if called returns to superior element)
	static struct MenuElement Return = {PageFirstElement, "Return", 6, 0, {NULL, NULL, NULL}, NULL};
		
	// Elements of Menu
	static struct MenuElement StartGame = {PageFirstElement, "Start Game", 10, 1, {&Return, NULL, NULL}, &Menu};
	static struct MenuElement Settings = {PageSecondElement, "Settings", 8, 1, {&Return, NULL, NULL}, &Menu};
	Menu.elements[0] = &StartGame;
	Menu.elements[1] = &Settings;
	
	// Print menu
	SelectedMenu = &Menu;
	OLED_menu();
	
	GameStarted = 0;
}

// Goes to sub-menu of element where the arrow is pointing
void OLED_menu_select_element(){
	
	struct MenuElement* nextSelectedMenu = SelectedMenu->elements[ArrowPositionNumber/2-1];
	
	if (nextSelectedMenu->name == "Return") SelectedMenu = SelectedMenu->superiorElement;	// If return: go to superior element
	else SelectedMenu = nextSelectedMenu;
	
	// Print SelectedMenu
	OLED_menu();
	
}

// Takes the joystick position and moves the arrow up and down the menu
void OLED_operate_menu(uint8_t joystick_position){
	uint8_t ArrowUp = ArrowPositionNumber - 2;
	uint8_t ArrowDown = ArrowPositionNumber + 2;
	if (SelectedMenu->numberElements > 0){
		if (joystick_position == 4 && (ArrowDown/2 <= SelectedMenu->numberElements)){ //DOWN
			OLED_move_arrow(ArrowDown);
		}
		else if (joystick_position == 2 && (ArrowUp >= PageFirstElement)){ //UP
			OLED_move_arrow(ArrowUp);
		}
	}
}

void OLED_page_selector(){
	printf("%u row is selected \r\n", ArrowPositionNumber);
}


// Print arrow at the first element in menu
void OLED_initialize_arrow_position(){
	
	ArrowPositionNumber = PageFirstElement;
	OLED_print_arrow(MinCollum, ArrowPositionNumber);
}

// Print arrow at given position
void OLED_print_arrow(uint8_t collum, uint8_t row){
	printf("%d, %d", collum, row);
	OLED_set_cursor(collum, 127, row, 0x07);
	OLED_write_data (0b00011000);
	OLED_write_data (0b00011000);
	OLED_write_data (0b01111110);
	OLED_write_data (0b00111100);
	OLED_write_data (0b00011000);
}

// Clear arrow at given position
void OLED_clear_arrow(uint8_t collum, uint8_t row){
	printf("Clear arrow");
	OLED_set_cursor(collum, 127, row, 0x07);
	OLED_write_data (0x00);
	OLED_write_data (0x00);
	OLED_write_data (0x00);
	OLED_write_data (0x00);
	OLED_write_data (0x00);
}

// Sets arrow at arrow_row and clears old arrow
void OLED_move_arrow (uint8_t arrow_row){
	OLED_clear_arrow(MinCollum, ArrowPositionNumber);
	OLED_print_arrow(MinCollum, arrow_row);
	ArrowPositionNumber = arrow_row;
}

void OLED_print_score(uint8_t score){
	
	OLED_menu();
	
	//OLED_flush(0);			// Turn all pixels off
	//OLED_reset_cursor();
	if (GameStarted){
		OLED_set_cursor(MenuCollum, MaxCollum, PageSecondElement, MaxPage);
		OLED_write_string("Score: ", 7, 8);
	
		uint8_t length;
	
		if (score < 10){
			length = 1;
		}
		else if (score < 100){
			length = 2;
		}
		else{
			length = 3;
		}
	
		char string[3];
		sprintf(string, "%d", score);
		OLED_write_string(string, length, 8);
	
	}

}



