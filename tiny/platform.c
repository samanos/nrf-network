#include <util/delay.h>

#include "softuart.h"
#include "utility.h"
#include "tiny_spi.h"

void print(char *str)
{
    #ifdef UART_ENABLED
    for (uint8_t i = 0; str[i] != 0; i++) {
        uartSend(str[i]);
    }
    #endif
}

void println(char *str)
{
    #ifdef UART_ENABLED
    print(str);
    uartSend('\n');
    uartSend('\r');
    #endif
}

void print_buf(uint8_t *buf, uint8_t n)
{
    #ifdef UART_ENABLED
    for (uint8_t i = 0; i < n; i++) {
        uartSend(hexChar(buf[i] >> 4));
        uartSend(hexChar(buf[i]));
    }
    uartSend('\n');
    uartSend('\r');
    #endif
}

void delay_ms(int ms)
{
    _delay_ms(ms);
}

void enable_spi()
{
    tiny_spi_begin();
}

void disable_spi()
{
}

uint8_t spi_transfer(uint8_t data)
{
    return tiny_spi_transfer(data);
}

void spi_transfern(uint8_t *buf, uint8_t n)
{
    tiny_spi_transfern(buf, n);
}

void ce_high()
{
    // CE is conntected to VCC.
}
