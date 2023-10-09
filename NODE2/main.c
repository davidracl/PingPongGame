/*
 * NODE2.c
 *
 * Created: 09.10.2023 08:28:03
 * Author : klevisr
 */ 


#include "sam.h"
#include "can_controller.h"
#include "printf-stdarg.h" 
#include "can_interrupt.h"
#include "uart.h"

#define LED1 42
#define LED2 43

int a = 0;

int main ( void ) {
	PIOA->PIO_PER |= PIO_PDR_P20; //enable pin A20. PIN43
	PIOA->PIO_PER |= PIO_PDR_P19; //enable pin A19, PIN42
	PIOA->PIO_OER |= PIO_PDR_P20; //set as output pin A20. PIN43
	PIOA->PIO_OER |= PIO_PDR_P19; //set as output pin A19, PIN42

	PIOA->PIO_SODR |= PIO_PDR_P20; //set pin A20. PIN43
	PIOA->PIO_SODR |= PIO_PDR_P19; //set pin A19, PIN42
	//PIOA->PIO_CODR |= PIO_PDR_P20; //clear pin A20. PIN43
	//PIOA->PIO_CODR |= PIO_PDR_P19; //clear pin A19, PIN42
	/*can_init () ;
	struct CAN_Message message ;
	message . id = 3;
	message . data_length = 1;
	message . data [0] = "U";
	can_message_send (& message ) ;
	*/
	while(1)
	{
		for (int i = 0; i < 100000; i++){
			a ++;
		}
		PIOA->PIO_CODR |= PIO_PDR_P20; //clear pin A20. PIN43
		PIOA->PIO_CODR |= PIO_PDR_P19; //clear pin A19, PIN42
		for (int i = 0; i < 100000; i++){
			a --;
		}
		PIOA->PIO_SODR |= PIO_PDR_P20; //set pin A20. PIN43
		PIOA->PIO_SODR |= PIO_PDR_P19; //set pin A19, PIN42
	}
	
}

void can_message_send ( struct CAN_Message * msg ) {
	uint8_t i;
	for (i = 0; i < msg -> data_length ; i ++);
}
