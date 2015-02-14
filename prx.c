#include <bcm2835.h>
#include <stdio.h>

#include "common.h"

int main(int argc, char **argv)
{
    if (!bcm2835_init())
        return 1;

    enable_spi();
    common_config();
    prx();
    prx_addr();

    ce_high();
    power_up();

    // Set autoack payload for data pipe 0
    char buf[6];
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
