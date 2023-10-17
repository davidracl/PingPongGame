#include "ir.h"

void ir_init(){
	PMC->PMC_PCER1 |= PMC_PCER1_PID37;  // ADC power on
	ADC->ADC_CR = ADC_CR_SWRST;		// Reset ADC
	
	ADC->ADC_MR |=  ADC_MR_TRGEN | ADC_MR_TRGSEL_ADC_TRIG1 | ADC_MR_SLEEP_NORMAL | ADC_MR_FREERUN_ON;
	 
	
	// ADC->ADC_CR |= ADC_CR_START;
	
	ADC->ADC_EMR |= ADC_EMR_CMPSEL(7) // // enable comparison only for channel 7
				| ADC_EMR_CMPMODE_LOW; // mode LOW (Generates an event when the converted data is lower than the low threshold of the window)
				// | ADC_EMR_CMPFILTER(10); // Number of consecutive compare events necessary 
	
	ADC->ADC_CWR = ADC_CWR_LOWTHRES(1500);
	
	ADC->ADC_CHER |= ADC_CHER_CH7; // enable channel
	
	ADC->ADC_IER |= ADC_IER_COMPE;// | ADC_IER_EOC7; // enable comparison interrupt
	NVIC_EnableIRQ(ADC_IRQn);
	NVIC_SetPriority(ADC_IRQn, 3);
	
	Score = 0;
	
}

int ir_read_value(){
	int result = ADC->ADC_CDR[7];
	printf("ADC: %d\r\n", result);
	return result;
}

void  ir_count_score( ){
		Score ++;
		printf("Score: %u\r\n", Score);	
}