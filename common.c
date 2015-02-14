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
    buf[1] |= 1 << 1; // PWR_UP = 1
    bcm2835_spi_transfern(buf, 2);
}

void power_down()
{
    // read CONFIG register
    char buf[2] = { 0, 0 };
    bcm2835_spi_transfern(buf, 2);

    buf[0] = 0b00100000; // write to CONFIG
    buf[1] |= 0 << 1; // PWR_UP = 0
    bcm2835_spi_transfern(buf, 2);
}

uint8_t rx_data_ready()
{
    uint8_t status = bcm2835_spi_transfer((uint8_t)0xFF);
    // RX_DR is bit 6 in status register
    return (status >> 5) & 1;
}

uint8_t get_rx_data(char *rx_data)
{
    // R_RX_PL_WID command
    char buf[32] = { 0b01100000, 0 };
    bcm2835_spi_transfern(buf, 2);

    uint8_t length = buf[1];

    // R_RX_PAYLOAD command
    rx_data[0] = 0b01100001;
    bcm2835_spi_transfern(rx_data, length + 1); // one for status byte

    return length;
}

void flush_tx()
{
    // FLUSH_TX command
    uint8_t status = bcm2835_spi_transfer((uint8_t) 0b11100001);
}

void flush_rx()
{
    // FLUSH_RX command
    uint8_t status = bcm2835_spi_transfer((uint8_t) 0b11100010);
}

void clean_int_flags()
{
    // STATUS register
    // RX_DR bit6, TX_DS bit5, MAX_RT bit4
    char buf[2] = { 0b00100111, 0b01110000 };
    bcm2835_spi_transfern(buf, 2);
}

void clean_up()
{
    flush_tx();
    flush_rx();
    clean_int_flags();
}
