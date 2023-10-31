#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

#define JoystickMiddle 50
#define JoystickLowerBound 25
#define JoystickUpperBound 75

struct adc_output
{
	uint8_t joystick_y;
	uint8_t joystick_x;
	uint8_t left_slider;
	uint8_t right_slider;
};

enum joystickPositionEnum {
	NEUTRAL,
	RIGHT,
	UP,
	LEFT,
	DOWN,
};

struct joystickPosition
{
	double xNormalized;
	double yNormalized;
	double angle;
	double distance;
	enum joystickPositionEnum position;
};

struct slidersPosition
{
	uint8_t left;
	uint8_t right;
};

void adc_init();
void PWM_init();
struct adc_output adc_read();
void setup_joystick();
int normalize_position(uint8_t currentValue, uint8_t centerValue, uint8_t minValue, uint8_t maxValue);
void get_joystick_position(struct joystickPosition* position);
struct slidersPosition get_sliders_position();


