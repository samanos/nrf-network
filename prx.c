#include <bcm2835.h>
#include <stdio.h>

#include "common.h"

int main(int argc, char **argv)
{
    // If you call this, it will not actually access the GPIO
// Use for testing
//        bcm2835_set_debug(1);

    if (!bcm2835_init())
        return 1;

    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536); // The default
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default

    // Send a byte to the slave and simultaneously read a byte back from the slave
    // If you tie MISO to MOSI, you should read back what was sent
    uint8_t data = bcm2835_spi_transfer(0xFF);
    printf("Status Register: %02X\n", data);

    // CONFIG
    // no interrupts, crc 2 bytes, PRX
    char buf[6] = { 0b00100000, 0b01111101 };
    bcm2835_spi_transfern(buf, 2);

    // EN_AA
    // autoack only on data pipe 0
    buf[0] = 0b00100001; buf[1] = 0b00000001;
    bcm2835_spi_transfern(buf, 2);

    // EN_RXADDR
    // enable only data pipe 0
    buf[0] = 0b00100010; buf[1] = 0b00000001;
    bcm2835_spi_transfern(buf, 2);

    // RF_CH
    // set channel to 0001000
    buf[0] = 0b00100101; buf[1] = 0b00001000;
    bcm2835_spi_transfern(buf, 2);

    // RX_ADDR_P0
    // receive address of data pipe 0
    buf[0] = 0b00101010; buf[1] = 't'; buf[2] = 'e'; buf[3] = 'm'; buf[4] = 'p'; buf[5] = '1';
    bcm2835_spi_transfern(buf, 6);

    // DYNPD
    // enable dynamic payload length for data pipe 0
    buf[0] = 0b00111100; buf[1] = 0b00000001;
    bcm2835_spi_transfern(buf, 2);

    // FEATURE
    // enable dyn payload length, enable payload with ack, enable no_ack command
    buf[0] = 0b00111101; buf[1] = 0b00000111;
    bcm2835_spi_transfern(buf, 2);

    // Put GPIO22/CE/P1-15 high
    bcm2835_gpio_fsel(RPI_GPIO_P1_15, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_set(RPI_GPIO_P1_15);

    // POWER UP
    power_up();

    // Set autoack payload for data pipe 0
    buf[0] = 0b10101000; buf[1] = 'o'; buf[2] = 'h'; buf[3] = 'i'; buf[4] = '!';
    bcm2835_spi_transfern(buf, 5);

    // read register
    buf[0] = 0b0000000; buf[1] = 0; buf[2] = 0; buf[3] = 0; buf[4] = 0; buf[5] = 0;
    bcm2835_spi_transfern(buf, 6);

    printf("Config: ");
    int i;
    for (i = 0; i < 6; i++) {
      printf("%02X", buf[i]);
    }
    printf("\n");
    for (i = 0; i < 6; i++) {
        printf("%c", buf[i]);
    }

    printf("\n");

    bcm2835_spi_end();
    bcm2835_close();
    return 0;
}

