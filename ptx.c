#include <bcm2835.h>
#include <stdio.h>

#include "common.h"

void tx_payload()
{
    // W_TX_PAYLOAD command
    char buf[11] = { 0b10100000, 'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd' };
    bcm2835_spi_transfern(buf, 11);
}

uint8_t max_retransmits_reached()
{
    uint8_t status = bcm2835_spi_transfer((uint8_t)0xFF);
    // MAX_RT is bit 4 in status register
    return (status >> 3) & 1;
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

    clean_up();

    receiver_addr();

    tx_payload();
    while (!rx_data_ready()) { }

    printf("Data ready, reading...");
    char buf[32];
    uint8_t length = get_rx_data(buf);

    int i;
    for (i = 1; i <= length; i++) {
        printf("%c", buf[i]);
    }
    printf("\n");

    clean_up();
    power_down();
    disable_spi();

    return 0;
}
