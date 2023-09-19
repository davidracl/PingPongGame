#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

struct adc_output
{
	uint8_t joystick_y;
	uint8_t joystick_x;
	uint8_t left_slider;
	uint8_t right_slider;
};
void adc_init (void);
struct adc_output adc_read(); //volatile
void initPWM();