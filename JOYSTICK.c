uint8_t originJoystickPositionX = 0;
uint8_t originJoystickPositionY = 0;

enum joystickPositionEnum {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NEUTRAL
};
	
struct joystickPosition
{
	double xNormalized;
	double yNormalized;
	double angle;
	double distance;
	enum joystickPositionEnum position;
};
	

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
	volatile double distance = curVa * 1.0 - centerValue;
	if(distance > 0)
	{
		return distance / (maxValue - centerValue);
	}
	else if(distance < 0) {
		return distance / (centerValue - minValue);
	}
	
	return 0;
}

struct joystickPosition get_joystick_position()
{
		struct adc_output newValues;
		struct joystickPosition position;
		double angle;
		
		newValues = adc_read();
		position.xNormalized = normalize_position(newValues.joystick_x, originJoystickPositionX, 0, 255);
		position.yNormalized = normalize_position(newValues.joystick_y, originJoystickPositionY, 0, 255);
		position.angle = get_joystick_angle(position.xNormalized, position.yNormalized);
		angle = position.angle;
		position.distance = get_joystick_distance(position.xNormalized, position.yNormalized);

		position.position = NEUTRAL;
			
		if(position.distance > 0.2)
		{
			if(angle > M_PI_4 && angle < 3*M_PI_4)
			{
				position.position = UP;
			}
			else if(angle > 3*M_PI_4 || angle < -3*M_PI_4)
			{
				position.position = LEFT;
			}
			else if(angle > -3*M_PI_4 && angle < -M_PI_4)
			{
				position.position = DOWN;
			} else {
				position.position = RIGHT;
			}
		}
		
		return position;
		
}
