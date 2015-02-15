#include <bcm2835.h>
#include <stdio.h>
#include <time.h>

void print(char *str)
{
    printf("%s", str);
}

void println(char *str)
{
    printf("%s\n", str);
}

void print_buf(uint8_t *buf, uint8_t n)
{
    for (uint8_t i = 0; i < n; i++) {
        printf("%c", buf[i]);
    }
    print("\n");
}

void delay_ms(int ms)
{
    nanosleep((struct timespec[]){{0, ms * 1000000}}, NULL);
}

void enable_spi()
{
    if (!bcm2835_init()) {
        println("Unable to init bcm2835.");
        return;
    }

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

uint8_t spi_transfer(uint8_t data)
{
    return bcm2835_spi_transfer(data);
}

void spi_transfern(uint8_t *buf, uint8_t n)
{
    bcm2835_spi_transfern(buf, n);
}

void ce_high()
{
    // Put GPIO22/CE/P1-15 high
    bcm2835_gpio_fsel(RPI_GPIO_P1_15, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_set(RPI_GPIO_P1_15);
}
