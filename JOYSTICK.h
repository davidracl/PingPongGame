#include "ADC.h"
#include "JOYSTICK.c"

enum joystickPositionEnum;
struct joystickPosition;
struct adc_output;


void setup_joystick();
double get_joystick_angle(double joystick_x, double joystick_y);
double get_joystick_distance(double joystick_x, double joystick_y);
double normalize_position(uint8_t currentValue, uint8_t centerValue, uint8_t minValue, uint8_t maxValue);
struct joystickPosition get_joystick_position();
