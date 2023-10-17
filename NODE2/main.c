/*
 * NODE2.c
 *
 * Created: 09.10.2023 08:28:03
 * Author : klevisr
 */ 
//#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "uart.h"
#include "servo.h"

#define LED1 42
#define LED2 43

#define F_CPU 84000000

int a = 0;
int adc_value;
int TimeSinceLastScore;

uint8_t FlagBallDetected;
uint8_t FlagBallInGame;

int main ( void ) {
	SystemInit();
	uart_init(F_CPU, 115200);		// Initialize uart with baud rate 115200
	servo_init();
	//servo_set_angle(0);
	ir_init();
	ADC->ADC_CR |= ADC_CR_START;
	delay();
	volatile int someValue = ir_read_value();
	TimeSinceLastScore = 0;
	
	// Baud rate: BRP = 20, SJW= 3, PRPAG= 1, PHASE1 = 5, PHASE2 = 6
	// equivalent to baud rate of 250 kbit/s
	can_init_def_tx_rx_mb(0x00143156);
	
	// Enable LEDs
	PIOA->PIO_PER |= PIO_PDR_P20; //enable pin A20. PIN43
	PIOA->PIO_PER |= PIO_PDR_P19; //enable pin A19, PIN42
	PIOA->PIO_OER |= PIO_PDR_P20; //set as output pin A20. PIN43
	PIOA->PIO_OER |= PIO_PDR_P19; //set as output pin A19, PIN42

	PIOA->PIO_SODR |= PIO_PDR_P20; //set pin A20. PIN43
	PIOA->PIO_SODR |= PIO_PDR_P19; //set pin A19, PIN42
	
	
	// Example message
	struct CAN_Message message;
	//struct CAN_Message receive_message0 ;
	//struct CAN_Message receive_message1 ;
	message.id = 3;
	message.data_length = 3;
	message.data[0] = 0x01;
	message.data[1] = 0x07;
	message.data[2] = 0x03;
	can_send(&message, 0);
	
	while(1)
	{
		//can_send(&message, 0);
		CAN0_Handler();
		//adc_value = ir_read_value();
		
		// If Ball out of bounds: increase score
		if (FlagBallDetected){
			ir_count_score();
			FlagBallDetected = 0;
		}
		
		// Delay
		for (int i = 0; i < 100000; i++){
			a ++;
		}
		/*
		PIOA->PIO_CODR |= PIO_PDR_P20; //clear pin A20. PIN43
		PIOA->PIO_CODR |= PIO_PDR_P19; //clear pin A19, PIN42
		*/
		for (int i = 0; i < 100000; i++){
			a --;
		}
		/*
		PIOA->PIO_SODR |= PIO_PDR_P20; //set pin A20. PIN43
		PIOA->PIO_SODR |= PIO_PDR_P19; //set pin A19, PIN42*/
		//can_send(&message, 0);
		/*
		can_receive(&receive_message0, 1);
		can_receive(&receive_message1, 2);
		printf("Received message buffer 0\r\nID: %d, Data: 0x%x\r\n", receive_message0.id, receive_message0.data[0]);
		printf("Received message buffer 1\r\nID: %d, Data: 0x%x\r\n", receive_message1.id, receive_message1.data[0]);
		*/
	
	}
	
	
	
}
volatile int test = 5;

void delay(){
	for (int i = 0; i < 100000; i++){
		test = 5;
	}
}

volatile int lastConversion;

void ADC_Handler() {
	lastConversion = ADC->ADC_CDR[7];
	FlagBallDetected = 1;
	ADC->ADC_ISR;  
}


/*
void can_message_send ( struct CAN_Message * msg ) {
	uint8_t i;
	for (i = 0; i < msg -> data_length ; i ++);
	
}*/
