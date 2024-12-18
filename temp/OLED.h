#pragma once
#include <stdint.h>
#include <avr/delay.h>


void OLED_init(); // PDF:"OLED LY190-128064" section 9.4
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

void OLED_write_string(char* data, uint32_t length);
void OLED_set_cursor(uint8_t collumn_start, uint8_t collumn_end, uint8_t page_start, uint8_t page_end);
void OLED_menu();

/*
void OLED_printf();
void OLED_Transmit(unsigned char data);
unsigned char OLED_Receive( void );
*/


