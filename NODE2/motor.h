#include  <stdio.h>
#include "sam.h"

#define MotorSpeed 1000
#define PI_Kp 20
#define PI_Ti 0.1
#define PI_Ki 0.1

float error_sum;
int16_t pos;


void motor_init();
void motor_control(int joystick_input);
void motor_set_speed(int speed);
void motor_set_direction(int right);
int16_t motor_read_encoder();
int16_t motor_control_position_PI(int reference);
void motor_control_speed(int joystick_input);