#include "OLED.c"

void OLED_init(); // PDF:"OLED LY190-128064" section 9.4
void OLED_reset();
void OLED_home();
void OLED_goto_line( );
void OLED_clear_line();
void OLED_pos();
void OLED_write_data(char data); //volatile
void OLED_write_command(char data); //volatile
void OLED_print(char*);
void OLED_set_brightness();