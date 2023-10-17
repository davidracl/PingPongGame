#include  <stdio.h>
#include "sam.h"

uint8_t currentAngle;

void servo_init();
void servo_set_angle(int angleDeg);
void move_servo(char joystick_position);