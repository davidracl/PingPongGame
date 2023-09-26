#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

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



struct adc_output
{
	uint8_t joystick_y;
	uint8_t joystick_x;
	uint8_t left_slider;
	uint8_t right_slider;
};

struct slidersPosition
{
	uint8_t left;
	uint8_t right;
};

void adc_init (void);
struct adc_output adc_read(); //volatile
double get_joystick_angle(double joystick_x, double joystick_y);
double get_joystick_distance(double joystick_x, double joystick_y);
void setup_joystick();
struct slidersPosition get_sliders_position();
/*
struct adc_output
{
	uint8_t joystick_y;
	uint8_t joystick_x;
	uint8_t left_slider;
	uint8_t right_slider;
};

//enum joystickPositionEnum;
//struct joystickPosition;

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

void adc_init (void);
struct adc_output adc_read(); //volatile
void initPWM();

void setup_joystick();
double get_joystick_angle(double joystick_x, double joystick_y);
double get_joystick_distance(double joystick_x, double joystick_y);
double normalize_position(uint8_t currentValue, uint8_t centerValue, uint8_t minValue, uint8_t maxValue);
struct joystickPosition* get_joystick_position();

struct slidersPosition;
struct slidersPosition get_sliders_position();

*/