#include <bcm2835.h>
#include <stdio.h>

#include "common.h"

void tx_payload()
{
    // W_TX_PAYLOAD command
    char buf[11] = { 0b10100000, 'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd' };
    bcm2835_spi_transfern(buf, 11);
}

int main(int argc, char **argv)
{
    if (!bcm2835_init())
        return 1;

    enable_spi();
    common_config();
    ptx();

    ce_high();
    power_up();

    receiver_addr();

    tx_payload();
    while (1) {
        uint8_t status = bcm2835_spi_transfer((uint8_t)0xFF);
        printf("Status %02X\n", status);
        sleep(1);
    }

    disable_spi();
    return 0;
}
