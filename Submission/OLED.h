#pragma once
#include <stdio.h>
#include <stdint.h>
#include <avr/delay.h>

#define MenuCollum 0x08
#define MinCollum 0x00
#define MaxCollum 127
#define MinPage 0x00
#define MaxPage 0x07

#define MenuMaxNumberOfELements 3
#define PageHeadline 0x00
#define PageFirstElement 0x02
#define PageSecondElement 0x04
#define PageThirdElement 0x06
#define MenuElementPageOffset 0x02

#define PixelBlockOn 0xFF
#define PixelBlockOff 0x00

struct MenuElement{
	uint8_t row;
	char* name;
	uint8_t lengthOfName;
	uint8_t numberElements;
	struct MenuElement* elements[MenuMaxNumberOfELements];
	struct MenuElement* superiorElement;
};

// Global variables 
uint8_t ArrowPositionNumber;		// Current position of arrow
struct MenuElement* SelectedMenu;	// Currently selected Menu (displayed)
uint8_t GameStarted;


// Command functions
void OLED_init(); // PDF:"OLED LY190-128064" section 9.4
void OLED_set_horizontal_mode();

void OLED_reset_cursor();
void OLED_set_cursor(uint8_t collumn_start, uint8_t collumn_end, uint8_t page_start, uint8_t page_end);

void OLED_write_command(uint8_t command);
void OLED_write_data(uint8_t data);
void OLED_write_string(char* data, uint32_t length, uint8_t font_size);

void OLED_flush(uint8_t on);

// Menu functions
void OLED_menu();
void OLED_menu_init();
void OLED_menu_select_element();
void OLED_operate_menu(uint8_t joystick_position);
void OLED_page_selector();

// Arrow functions
void OLED_initialize_arrow_position();
void OLED_print_arrow(uint8_t collum, uint8_t row);
void OLED_clear_arrow(uint8_t collum, uint8_t row);
void OLED_move_arrow (uint8_t arrow_row);

// Score functions
void OLED_print_score(uint8_t score);







