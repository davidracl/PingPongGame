#include "ADC.h"

void adc_init(void)
{
	//
	static uint8_t Configuration_Adc = 0x00;

}

struct adc_output adc_read()
{
	
	volatile char *adc = (char *) 0x1400; // Start address for the ADC
	adc[0] = 0x01;
	_delay_us(500);
	uint8_t joystick_y = adc[0];
	uint8_t joystick_x = adc[0];
	uint8_t left_slider = adc[0];
	uint8_t right_slider = adc[0];

	struct adc_output output = {joystick_x, joystick_y, left_slider, right_slider};
	
	return output;
	
}

void initPWM() {
	
	// PD4 OC3A (Timer/Counter3 Output Compare A Match Output
	
	// Set pP as an output pin
	DDRD |= (1 << PD4);
	
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);

	// Set non-inverting mode for OC3A (Clear on Compare Match)
	// Set Fast PWM mode with TOP value at ICR3
	
	TCCR3A |= (1 << WGM31) | (0 << WGM30) | (1 << COM3A1);
	TCCR3B |= (1 << WGM33) | (1 << WGM32) | (1 << CS30);
	
	ICR3 = 5;
	OCR3A = 2;
	
}

