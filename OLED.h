#pragma once
#include <stdio.h>
#include <stdint.h>
#include <avr/delay.h>



#define MenuCollum 0x08
#define MinCollum 0x00
#define MaxCollum 127
#define MinPage 0x00
#define MaxPage 0x07
#define PageMenu 0x00
#define PageStartGame 0x02
#define PageSettings 0x04

#define ManuMaxNumberOfELements 3
#define PageHeadline 0x00
#define PageFirstElement 0x02
#define PageSecondElement 0x04
#define PageThirdElement 0x06

#define PixelBlockOn 0xFF
#define PixelBlockOff 0x00


struct ArrowPosition{
	uint8_t row;
	uint8_t collumn;
};

struct MenuElement{
	uint8_t row;
	char* name;
	uint8_t lengthOfName;
	uint8_t numberElements;
	struct MenuElement* elements[ManuMaxNumberOfELements];
	struct MenuELement* superiorElement;
};

uint8_t ArrowPositionNumber;



void OLED_init(); // PDF:"OLED LY190-128064" section 9.4
void OLED_write_data(uint8_t data);
void OLED_write_command(uint8_t command);
void OLED_reset_cursor();
void OLED_set_cursor(uint8_t collumn_start, uint8_t collumn_end, uint8_t page_start, uint8_t page_end);
void OLED_flush(uint8_t on);
void OLED_set_horizontal_mode();
void OLED_write_string(char* data, uint32_t length, uint8_t font_size);
void OLED_menu();
void OLED_print_arrow(uint8_t collum, uint8_t row);
void OLED_clear_arrow(uint8_t collum, uint8_t row);
void OLED_initialize_arrow_position();
void OLED_page_selector();
struct MenuElement* OLED_menu_init();

void OLED_set_arrow_line (uint8_t arrow_row);



/*
void OLED_reset();
void OLED_home();
void OLED_goto_line( );
void OLED_clear_line();
void OLED_pos();
void OLED_write_data(uint8_t data); //volatile
void OLED_write_command(uint8_t data); //volatile
void OLED_print(char*);
void OLED_set_brightness();
void OLED_reset_cursor();
void OLED_set_horizontal_mode();

void OLED_write_string(char* data, uint32_t length, uint8_t font_size);
void OLED_set_cursor(uint8_t collumn_start, uint8_t collumn_end, uint8_t page_start, uint8_t page_end);
void OLED_menu();
*/
/*
void OLED_printf();
void OLED_Transmit(unsigned char data);
unsigned char OLED_Receive( void );
*/


