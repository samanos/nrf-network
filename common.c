#include <bcm2835.h>

void enable_spi()
{
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536); // The default
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
}

void disable_spi()
{
    bcm2835_spi_end();
    bcm2835_close();
}

void common_config()
{
    // CONFIG
    // no interrupts, crc 2 bytes
    char buf[2] = { 0b00100000, 0b01111100 };
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

    // DYNPD
    // enable dynamic payload length for data pipe 0
    buf[0] = 0b00111100; buf[1] = 0b00000001;
    bcm2835_spi_transfern(buf, 2);

    // FEATURE
    // enable dyn payload length, enable payload with ack, enable no_ack command
    buf[0] = 0b00111101; buf[1] = 0b00000111;
    bcm2835_spi_transfern(buf, 2);
}

void ptx()
{
    // read CONFIG register
    char buf[2] = { 0, 0 };
    bcm2835_spi_transfern(buf, 2);

    buf[0] = 0b00100000; // write to CONFIG
    buf[1] |= 0 << 0; // PRX
    bcm2835_spi_transfern(buf, 2);
}

void prx()
{
    // read CONFIG register
    char buf[2] = { 0, 0 };
    bcm2835_spi_transfern(buf, 2);

    buf[0] = 0b00100000; // write to CONFIG
    buf[1] |= 1 << 0; // PRX
    bcm2835_spi_transfern(buf, 2);
}

void prx_addr()
{
    // RX_ADDR_P0
    // receive address of data pipe 0
    char buf[6] = { 0b00101010, 't', 'e', 'm', 'p', '1' };
    bcm2835_spi_transfern(buf, 6);
}

void receiver_addr()
{
    // TX_ADDR
    // receiver address
    char buf[6] = { 0b00110000, 't', 'e', 'm', 'p', '1' };
    bcm2835_spi_transfern(buf, 6);

    // need to set receive address for data pipe 0 for acks
    prx_addr();
}

void ce_high()
{
    // Put GPIO22/CE/P1-15 high
    bcm2835_gpio_fsel(RPI_GPIO_P1_15, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_set(RPI_GPIO_P1_15);
}

void power_up()
{
    // read CONFIG register
    char buf[2] = { 0, 0 };
    bcm2835_spi_transfern(buf, 2);

    buf[0] = 0b00100000; // write to CONFIG
    buf[1] |= 1 << 1; // POWER UP
    bcm2835_spi_transfern(buf, 2);
}
