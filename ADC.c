#include "ADC.h"

uint8_t originJoystickPositionX = 0;
uint8_t originJoystickPositionY = 0;
	
void adc_init()
{
	static uint8_t Configuration_Adc = 0x00;
}

void PWM_init() {
	
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

struct adc_output adc_read()
{
	// Start address for the ADC
	volatile char *adc = (char *) 0x1400; 

	// Write to ADC to enable output
	adc[0] = 0x01;			
	_delay_us(500);

	// Read joystick and slider values
	uint8_t joystick_x = adc[0];	
	uint8_t joystick_y = adc[0];
	uint8_t left_slider = adc[0];
	uint8_t right_slider = adc[0];
	
	struct adc_output output = {joystick_x, joystick_y, left_slider, right_slider};
	return output;
}


void setup_joystick()
{
	// Save initial joystick position as origin
	struct adc_output initValues = adc_read();
	originJoystickPositionX = initValues.joystick_x;
	originJoystickPositionY = initValues.joystick_y;
	
}


int normalize_position(uint8_t currentValue, uint8_t centerValue, uint8_t minValue, uint8_t maxValue)
{
	int result;
	
	// Normalize the current value on a scale from 0-100 with 50 at the center
	if (currentValue > centerValue){
		result = 50 + ((currentValue - centerValue)*50) / (255 - centerValue);
	}
	else if (currentValue < centerValue){
		result = (currentValue * 50) / centerValue;
	}
	else result = 50;
	
	return result;
}


void get_joystick_position(struct joystickPosition* position)
{
	struct adc_output newValues;
	double angle;
	
	newValues = adc_read();	// Read ADC output
	
	// Normailze ADC output
	position->xNormalized = normalize_position(newValues.joystick_x, originJoystickPositionX, 0, 255);
	position->yNormalized = normalize_position(newValues.joystick_y, originJoystickPositionY, 0, 255);

	// Categorize output position
	position->position = NEUTRAL;
	
	if( position->xNormalized > JoystickUpperBound || position->xNormalized < JoystickLowerBound || position->yNormalized > JoystickUpperBound || position->yNormalized < JoystickLowerBound)
	{
		if(position->yNormalized > JoystickMiddle && (position->xNormalized > JoystickLowerBound && position->xNormalized <= JoystickUpperBound))
		{
			position->position = UP;
		}
		else if(position->xNormalized <= JoystickMiddle && (position->yNormalized > JoystickLowerBound && position->yNormalized <= JoystickUpperBound))
		{
			position->position = LEFT;
		}
		else if(position->yNormalized <= JoystickMiddle && (position->xNormalized > JoystickLowerBound && position->xNormalized <= JoystickUpperBound))
		{
			position->position = DOWN;
		} else {
			position->position = RIGHT;
		}
	}
	
	
}


struct slidersPosition get_sliders_position()
{
		struct adc_output newValues;	
		struct slidersPosition position;	

		newValues = adc_read();		// Read ADC output
		
		// Save sliders position
		position.left = newValues.left_slider;
		position.right = newValues.right_slider;
		
		return position;
};


