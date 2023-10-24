#include "solenoid.h"

void solenoid_init(){
	PIOC->PIO_PER |= PIO_PER_P13;	// Enable PIO output for PIN51 (solenoid)
													
	PIOC->PIO_OER |= PIO_OER_P13;
	PIOC->PIO_CODR |= PIO_CODR_P13; // Low as default value for solenoid 
	
}

void solenoid_control(){
	printf("Push button pressed!\r\n");
	PIOC->PIO_SODR |= PIO_SODR_P13; // Activate solenoid
	delay_micros(600);
	PIOC->PIO_CODR |= PIO_CODR_P13; 
}