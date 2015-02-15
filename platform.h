#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>

/**
 * Contains function definitions which implementations are platform specific.
 */

void print(char *str);
void println(char *str);
void print_buf(uint8_t *buf, uint8_t n);

void delay_ms(int ms);

void enable_spi();
void disable_spi();

uint8_t spi_transfer(uint8_t spiData);
void spi_transfern(uint8_t *buf, uint8_t n);

void ce_high();

#endif
