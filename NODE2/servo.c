#include "servo.h"

#define MAX_DUTY_COUNT 65000
#define MAX_ANGLE_DEG 180

void servo_init(){
	PMC->PMC_PCER1 |= PMC_PCER1_PID36; // PWM controller power ON
	PWM->PWM_DIS = PWM_DIS_CHID5;

	PMC->PMC_PCER1 |= PMC_PCER0_PID11;
	PIOC->PIO_PDR |= PIO_PDR_P19;
	PIOC->PIO_ABSR |= PIO_ABSR_P19;

	PWM->PWM_CLK |= PWM_CLK_DIVA(13) | PWM_CLK_PREA(1); 
	PWM->PWM_CH_NUM[5].PWM_CMR |= PWM_CMR_CPRE_CLKA;
	PWM->PWM_CH_NUM[5].PWM_CPRD |= MAX_DUTY_COUNT;
	currentAngle = 90;
	servo_set_angle(currentAngle);
	PWM->PWM_ENA |= PWM_ENA_CHID5;
}

void servo_set_angle(int angleDeg){
	if (angleDeg < 0){
		angleDeg = 0;
	}
	else if (angleDeg > 180){
		angleDeg = 180;
	}
	// 0 = 1 ms,
	// 90 = 1.5 ms
	// 180 deg = 2 ms;
	int dutyCycle = (MAX_DUTY_COUNT / 20 * angleDeg)/MAX_ANGLE_DEG + MAX_DUTY_COUNT/20;
	
	PWM->PWM_CH_NUM[5].PWM_CDTY = MAX_DUTY_COUNT - dutyCycle;
}

void move_servo(char joystick_position){
	int newAngle = currentAngle;
	if (joystick_position == 2){ //UP
		newAngle ++;		// increase if faster
	} else if (joystick_position == 4){ //DOWN
		newAngle --;
	}
	
	if (newAngle != currentAngle){
		if (newAngle < 0){
			newAngle = 0;
		}
		else if (newAngle > 180){
			newAngle = 180;
		}
	
		servo_set_angle(newAngle);
		currentAngle = newAngle;
	}
	//printf("Current angle: %d\n\r", currentAngle);

}