#ifndef TINY_SPI_H
#define TINY_SPI_H

#include <avr/io.h>

#define SPI_PORT PORTB
#define SPI_DDR_PORT DDRB
#define USCK_DD_PIN DDB2
#define DO_DD_PIN DDB1
#define DI_DD_PIN DDB0

#define SPI_CSN_PIN DDB3

void tiny_spi_begin();
uint8_t tiny_spi_transfer(uint8_t spiData);
void tiny_spi_transfern(uint8_t *buf, uint8_t n);

#endif
