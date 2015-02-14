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
    uint8_t status = bcm2835_spi_transfer((uint8_t) 0xFF);
    // MAX_RT is bit 4 in status register
    return (status >> 4) & 1;
}

uint8_t tx_data_sent()
{
    uint8_t status = bcm2835_spi_transfer((uint8_t) 0xFF);
    // TX_DS is bit 5 in status register
    return (status >> 5) & 1;
}

void clean_max_rt_int()
{
    uint8_t status = bcm2835_spi_transfer((uint8_t) 0xFF);
    status |= 1 << 4;

    // write to STATUS register
    char buf[2] = { 0b00100111, status };
    bcm2835_spi_transfern(buf, 2);
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
    while (!tx_data_sent() && !max_retransmits_reached()) {}

    if (max_retransmits_reached()) {
        printf("Unable to send data.\n");
    }
    else if (!rx_data_ready()) {
        printf("Empty ack this time!\n");
    }
    else {
        printf("Data ready, reading...");

        uint8_t status = bcm2835_spi_transfer((uint8_t) 0xFF);
        printf("Status %02X\n", status);

        char buf[32];
        uint8_t length = get_rx_data(buf);

        int i;
        for (i = 1; i <= length; i++) {
            printf("%c", buf[i]);
        }
        printf("\n");
    }

    clean_up();
    power_down();
    disable_spi();

    return 0;
}
