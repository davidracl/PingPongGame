void OLED_init()
{
	OLED_write_command(0xae); // display off
	OLED_write_command(0xa1); //segment remap
	OLED_write_command(0xda); //common pads hardware: alternative
	OLED_write_command(0x12);
	OLED_write_command(0xc8); //common output scan direction:com63~com0
	OLED_write_command(0xa8); //multiplex ration mode:63
	OLED_write_command(0x3f);
	OLED_write_command(0xd5); //display divide ratio/osc. freq. mode
	OLED_write_command(0x80);
	OLED_write_command(0x81); //contrast control
	OLED_write_command(0x50);
	OLED_write_command(0xd9); //set pre-charge period
	OLED_write_command(0x21);
	OLED_write_command(0x20); //Set Memory Addressing Mode
	OLED_write_command(0x02);
	OLED_write_command(0xdb); //VCOM deselect level mode
	OLED_write_command(0x30);
	OLED_write_command(0xad); //master configuration
	OLED_write_command(0x00);
	OLED_write_command(0xa4); //out follows RAM content
	OLED_write_command(0xa6); //set normal display
	OLED_write_command(0xaf); // display on
	
	OLED_write_command(0x22);
	OLED_write_data(0x00);
	OLED_write_data(0x00);
	
	OLED_write_command(0x21);
	OLED_write_data(0x00);
	OLED_write_data(0x00);
	
	OLED_write_data(0xff);
		OLED_write_data(0xff);
	OLED_write_data(0xff);
	OLED_write_data(0xff);
	OLED_write_data(0xff);
	OLED_write_data(0xff);
	OLED_write_data(0xff);
	OLED_write_command(0xa4);




	
	
}

void OLED_write_data(uint8_t data){
	volatile char *oled = (char *) 0x1200; // Start address for the ADC
	oled[0] = data;
	volatile uint8_t test = 0;
	test = test + 2;
}

void OLED_write_command(uint8_t data){
	volatile char *oled = (char *) 0x1000; // Start address for the ADC
	oled[0] = data;
	volatile uint8_t test = 0;
	test = test + 2;
}
