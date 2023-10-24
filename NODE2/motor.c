#include "motor.h"

void motor_init(){
	PMC->PMC_PCER1 |= PMC_PCER1_PID38; // enable DAC peripheral
	DACC->DACC_CR |= DACC_CR_SWRST;
	
	//PMC->PMC_PCER1 |= PMC_PCER0_PID14; // enable PIOD 
	
	
	PIOD->PIO_PER |= PIO_PER_P10 | PIO_PER_P9;		// Enable PIO output for PD10 (= PIN32 = DIR for motor box)
													// Enable PIO output for PD9 (= PIN30 for ENA for motor box)
													
	PIOD->PIO_OER |= PIO_OER_P10 | PIO_OER_P9;
	PIOD->PIO_CODR |= PIO_CODR_P10; // Low as default value for direction motor
	PIOD->PIO_SODR |= PIO_SODR_P9; // High as default value for enable motor
	
	// Enable motor box readings										
	
	PIOD->PIO_PER |= PIO_PER_P0 | PIO_PER_P1 | PIO_PER_P2;		// enable PIO output for NOT_OE, NOT_RST, SEL (PIN25, PIN26, PIN27)
	PIOD->PIO_OER |= PIO_OER_P0 | PIO_OER_P1 | PIO_OER_P2;		// enable output write
	
	PIOD->PIO_CODR |= PIO_CODR_P1;									// reset
	PIOD->PIO_SODR |= PIO_SODR_P1;									// Disable reset
	
	/*
	PIOC->PIO_PER = PIO_PER_P1;
	PIOC->PIO_ODR = PIO_PER_P1;*/
	
	
	PIOC->PIO_PER |= PIO_PER_P1 | PIO_PER_P2 | PIO_PER_P3 | PIO_PER_P4 | PIO_PER_P5 | PIO_PER_P6 | PIO_PER_P7 | PIO_PER_P8; 
	PIOC->PIO_ODR = PIO_PER_P1 | PIO_PER_P2 | PIO_PER_P3 | PIO_PER_P4 | PIO_PER_P5 | PIO_PER_P6 | PIO_PER_P7 | PIO_PER_P8; 
	
	PMC->PMC_PCR |= PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_PIOC << PMC_PCR_PID_Pos);
	PMC->PMC_PCER0 |= PMC_PCER0_PID13; // enable PIOC for encoder data read

	//PIOC->PIO_PUDR = PIO_PER_P1 | PIO_PER_P2 | PIO_PER_P3 | PIO_PER_P4 | PIO_PER_P5 | PIO_PER_P6 | PIO_PER_P7 | PIO_PER_P8;
	
	// Setup DAC												
	DACC->DACC_MR |= DACC_MR_TAG_DIS | DACC_MR_USER_SEL_CHANNEL1;
	DACC->DACC_CHER |= DACC_CHER_CH1 | DACC_CHER_CH0; // CH1 => PB16
	DACC->DACC_CDR |= 0x00;
	
	pos = motor_read_encoder();
	error_sum = 0;
}

void motor_control(int joystick_input)
{
	//currentSpeed += speed_change;
	//DACC->DACC_CDR = abs(speed);
	motor_control_speed(joystick_input);
	
	/*
	if(joystick_input == 1){		// RIGHT
		motor_set_direction(1);
		motor_set_speed(MotorSpeed);
	} else if(joystick_input == 3){		//LEFT
		motor_set_direction(0);
		motor_set_speed(MotorSpeed);
	}
	else{
		motor_set_speed(0);
	}*/
}

void motor_set_speed(int speed){
	if (speed > 4095) speed = 4095;
	else if (speed < 0) speed = 0;
	
	DACC->DACC_CDR = speed;
	volatile uint16_t motorValue = motor_read_encoder();
}

void motor_set_direction(int right){
	if (right){
		PIOD->PIO_CODR |= PIO_CODR_P10;
	}
	else{
		PIOD->PIO_SODR |= PIO_SODR_P10;
	}
}

int16_t motor_read_encoder(){
	int16_t result;
	uint16_t first_8_bits_mask =  0xFF << 1;
	
	// 1
	PIOD->PIO_CODR |= PIO_CODR_P0; // output enable 0
	// 2
	PIOD->PIO_CODR |= PIO_CODR_P2;	// select 0
	// 3
	delay_micros(30);
	// 4
	result = (PIOC->PIO_PDSR & first_8_bits_mask) << 7;
	// 5 
	PIOD->PIO_SODR |= PIO_CODR_P2;	// select 1
	// 6 
	delay_micros(30);
	// 7
	result |= (PIOC->PIO_PDSR & first_8_bits_mask) >> 1;
	// 8
	PIOD->PIO_SODR |= PIO_CODR_P0; // output enable 1
	
	//printf("Motor value: %d\r\n", result);
	return result;
	
}

int16_t motor_control_position_PI(int reference){
	
	int16_t currentPos = motor_read_encoder();
		
	float error = reference - currentPos;
	error_sum += error;
	
	int16_t position_input = PI_Kp*error + PI_Ti*PI_Ki*error_sum;
	
	return position_input;
	
}

void motor_control_speed(int joystick_input){
	
	int16_t currentPos = motor_read_encoder();
	int8_t speed;
	
	//if (joystick_input == 1) pos += 10;
	//else if (joystick_input == 3) pos -= 10;
	
	if(joystick_input == 1){		// RIGHT
		motor_set_direction(1);
		pos += 10;
		speed = motor_control_position_PI(pos);
	} else if(joystick_input == 3){		//LEFT
		motor_set_direction(0);
		pos -= 10;
		speed = motor_control_position_PI(pos);
	}
	else{
		pos = currentPos;
		speed = 0;
	}
	
	
	motor_set_speed(abs(speed)*15);
	//motor_set_speed(abs(pos-currentPos)*100);
	pos = motor_read_encoder();
	
	//motor_set_speed(abs(pos-currentPos));
		
	/*
	printf("Motor position: %d, Pos: %d \r\n", motor_read_encoder(), pos);
	while (abs(motor_read_encoder() - pos) > 5 ){
		
		if (pos < -313){
			pos = -313;
		}
		else if (pos > 0){
			pos = 0;
		}
	
		int16_t position_n = motor_control_position_PI(pos);
		int16_t position_n1 = motor_control_position_PI(pos);
		volatile int16_t speed = (position_n1 - position_n)/PI_Ti;
		uint16_t direction = speed >> 15;
		printf("2 Motor position: %d, Pos: %d \r\n", motor_read_encoder(), pos);
		printf("Speed: %d, Direction: %d\r\n", speed, direction);
		motor_set_direction(!direction);
		motor_set_speed(abs(speed));		
		
	}*/
	
	
	pos = motor_read_encoder();

}
