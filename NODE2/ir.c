#include "ir.h"

void ir_init(){
	PMC->PMC_PCER1 |= PMC_PCER1_PID37;  // ADC power on
	
	ADC->ADC_MR |=  ADC_MR_TRGEN | ADC_MR_TRGSEL_ADC_TRIG1 | ADC_MR_SLEEP_NORMAL | ADC_MR_FREERUN_ON;
	NVIC_EnableIRQ(ADC_IRQn);
	 
	ADC->ADC_CHER |= ADC_CHER_CH7; // enable channel
	// ADC->ADC_CR |= ADC_CR_START;
}

int ir_read_value(){
	return ADC->ADC_CDR[7];
}