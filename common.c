#include <bcm2835.h>

void power_up()
{
    // read CONFIG register
    char buf[2] = { 0, 0 };
    bcm2835_spi_transfern(buf, 2);

    buf[0] = 0b00100000; // write to CONFIG
    buf[1] |= 1 << 1; // POWER UP
    bcm2835_spi_transfern(buf, 2);
}

