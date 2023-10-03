#include <avr/io.h>
#include <avr/interrupt.h>

void SPI_SlaveInit();
char SPI_SlaveReceive();
void SPI_MasterInit();
void SPI_init();
void SPI_MasterTransmit(char cData);