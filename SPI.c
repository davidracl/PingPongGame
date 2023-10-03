#include "SPI.h"

void SPI_init(){
	
	SPCR &= ~(1 << CPOL);	// Select SPI mode 0
	SPCR &= ~(1 << CPHA);
	SPI_MasterInit();
	//SPI_SlaveInit();
	DDRB |= (1<<PB4);
	PORTB |= (1<<PB4);
	//setup interrupt in PD3
	DDRD  &= ~(1 << PD3);     // set PD3 to input
	GICR |= (1<<INT1);      // Enable INT1 External Interrupt
	MCUCR |= (1<<ISC01);    // Falling-Edge Triggered INT1
	sei();     // Enable Interrupts	setup_joystick();
	
	// Set SS low
	// write data to SPI register (clock generator enabled)
	// After shift one byte: SPI clock generator stops, SPIF=1
	// If SPIE (interrupt) set in SPCR register
	
	
}
void SPI_MasterInit()
{
	/* Set MOSI and SCK output, all others input */
	DDRB |= (1<<DDB5)|(1<<DDB7);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}
void SPI_MasterTransmit(char cData)
{
	PORTB &= ~(1<<PB4);
	/* Start transmission */
	SPDR = cData;
	
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
	PORTB |= (1<<PB4);
}
void SPI_SlaveInit()
{
	/* Set MISO output, all others input */
	DDRB = (1<<DDB6);
	/* Enable SPI */
	SPCR = (1<<SPE);
}
char SPI_SlaveReceive()
{
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)))
	;
	/* Return data register */
	return SPDR;
}