#include "ADC.h"


	
	
void adc_init()
{
	// 
	static uint8_t Configuration_Adc = 0x00;
	 

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



struct adc_output adc_read()
{
	
	volatile char *adc = (char *) 0x1400; // Start address for the ADC
	adc[0] = 0x01;
	// tCONV = (9 x N x 2)/fCLK
	// (9*8*2)/819138 = 175.79455e-6
	_delay_us(500);
	uint8_t joystick_y = adc[0];
	uint8_t joystick_x = adc[0];
	uint8_t left_slider = adc[0];
	uint8_t right_slider = adc[0];
	printf("%02X, %02X, %02X, %02X \r\n", joystick_y, joystick_x, left_slider, right_slider);
	
	//double joystick_angle = get_joystick_angle(joystick_x, joystick_y);
	//printf("angle: %f", joystick_angle);
	
	 struct adc_output output = {joystick_x, joystick_y, left_slider, right_slider};
		 
	 return output;
	//return 0;
	
}

uint8_t originJoystickPositionX = 0;
uint8_t originJoystickPositionY = 0;

void setup_joystick()
{
	struct adc_output initValues = adc_read();
	originJoystickPositionX = initValues.joystick_x;
	originJoystickPositionY = initValues.joystick_y;
		
}

double get_joystick_angle(double joystick_x, double joystick_y)
{
	// in radians
	return atan2(joystick_x, joystick_y);
}

double get_joystick_distance(double joystick_x, double joystick_y)
{
	double pow_x = pow(joystick_x, 2);
	double pow_y = pow(joystick_y, 2);
	
	return sqrt(pow_x + pow_y);
}

double normalize_position(uint8_t currentValue, uint8_t centerValue, uint8_t minValue, uint8_t maxValue)
{
	volatile uint8_t curVa = currentValue;
	volatile double distance =  (double)((double)curVa  - (double)centerValue);

	if(distance > 0.0)
	{
		return distance / (((double)maxValue - (double)centerValue));
	}
	else if(distance < 0.0) {
		return distance / (((double)centerValue - (double)minValue));
	}
	
	return 0.0;
}

void get_joystick_position(struct joystickPosition* position)
{
		volatile struct adc_output newValues;
		//volatile struct joystickPosition position;
		double angle;
		volatile double debugX;
		volatile double debugY;
		volatile double debugDistance;
		do 
		{
			newValues = adc_read();
			
			position->xNormalized = normalize_position(newValues.joystick_x, originJoystickPositionX, 0, 255);
			position->yNormalized = normalize_position(newValues.joystick_y, originJoystickPositionY, 0, 255);
			debugX = position->xNormalized;
			debugY = position->yNormalized;
	
			position->angle = get_joystick_angle(position->xNormalized, position->yNormalized);
			angle = position->angle;
			position->distance = get_joystick_distance(position->xNormalized, position->yNormalized);

			debugDistance = position->distance;
			

		} while (position->distance >= 1.41421356236);

		position->position = NEUTRAL;
		
			/*LEFT,
			RIGHT,
			UP,
			DOWN,
			NEUTRAL*/
			
		if(position->distance > 0.2)
		{
			if(angle > M_PI_4 && angle < 3*M_PI_4)
			{
				printf("UP");
				position->position = UP;
			}
			else if(angle > 3*M_PI_4 || angle < -3*M_PI_4)
			{
								printf("LEFt");

				position->position = LEFT;
			}
			else if(angle > -3*M_PI_4 && angle < -M_PI_4)
			{
								printf("DOWN");

				position->position = DOWN;
			} else {
								printf("RIGHT");

				position->position = RIGHT;
			}
		}
		
		
}



struct slidersPosition get_sliders_position()
{
		struct adc_output newValues;	
		struct slidersPosition position;	
		newValues = adc_read();
		
		position.left = newValues.left_slider;
		position.right = newValues.right_slider;
		
		return position;
};
