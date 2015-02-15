#include <util/atomic.h>
#include <util/delay.h>

#include "tiny_spi.h"

void tiny_spi_begin(void)
{
    USICR &= ~(_BV(USISIE) | _BV(USIOIE) | _BV(USIWM1));
    USICR |= _BV(USIWM0) | _BV(USICS1) | _BV(USICLK);
    SPI_DDR_PORT |= _BV(USCK_DD_PIN);   //set the USCK pin as output
    SPI_DDR_PORT |= _BV(DO_DD_PIN);     //set the DO pin as output
    SPI_DDR_PORT &= ~_BV(DI_DD_PIN);    //set the DI pin as input

    USICR &= ~_BV(USICS0); // SPI MODE 0

    //SPI_DDR_PORT |= _BV(SPI_CSN_PIN);
    //SPI_PORT |= _BV(SPI_CSN_PIN);
}

uint8_t tiny_spi_crank(uint8_t spiData)
{
    USIDR = spiData;
    USISR = _BV(USIOIF);                //clear counter and counter overflow interrupt flag
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { //ensure a consistent clock period
        while ( !(USISR & _BV(USIOIF)) ) USICR |= _BV(USITC);
    }
    return USIDR;
}

void before_transfer()
{
    //SPI_PORT &= ~_BV(SPI_CSN_PIN);
    SPI_PORT &= ~_BV(USCK_DD_PIN);  // SCK low will pull CSN low as well
    _delay_us(16);  // allow CSN to settle
}

void after_transfer()
{
    //SPI_PORT |= _BV(SPI_CSN_PIN);
    SPI_PORT |= _BV(USCK_DD_PIN);  // SCK high will pull CSN high as well
    _delay_us(128);  // allow csn to settle
}

void tiny_spi_transfern(uint8_t *buf, uint8_t n)
{
    before_transfer();
    for (int i = 0; i < n; i++) {
        buf[i] = tiny_spi_crank(buf[i]);
    }
    after_transfer();
}

uint8_t tiny_spi_transfer(uint8_t data)
{
    before_transfer();
    uint8_t ret = tiny_spi_crank(data);
    after_transfer();
    return ret;
}
