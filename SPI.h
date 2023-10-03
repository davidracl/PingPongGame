#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t SPI_read();
void SPI_MasterInit();
void SPI_init();
void SPI_write(char cData);