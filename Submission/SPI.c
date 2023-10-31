#include "SPI.h"

void SPI_init(){
	SPI_MasterInit();
	
	SPCR &= ~(1 << CPOL);	// Select SPI mode 0
	SPCR &= ~(1 << CPHA);
	
	DDRB |= (1<<PB4);		// Set PB4 to input
	PORTB |= (1<<PB4);
	
	// Setup interrupt in PD3
	DDRD  &= ~(1 << PD3);		// Set PD3 to input
	GICR |= (1<<INT1);			// Enable INT1 External Interrupt
	MCUCR |= (1<<ISC01);		// Falling-Edge Triggered INT1
	sei();     
	
	
}
void SPI_MasterInit()
{
	/* Set MOSI and SCK output, all others input */
	DDRB |= (1<<DDB4)|(1<<DDB5)|(1<<DDB7);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_write(char cData)
{
	// Start transmission 
	SPDR = cData;
	
	// Wait for transmission complete 
	while(!(SPSR & (1<<SPIF)))
	;
}

uint8_t SPI_read()
{
	SPDR = 0xFF;
	
	// Wait for reception complete 
	while(!(SPSR & (1<<SPIF))) {
	}
	
	// Return data register 
	return SPDR;
}