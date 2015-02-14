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

        if (rx_data_ready()) {
            printf("Data ready, reading...");
            char buf[32];
            uint8_t length = get_rx_data(buf);

            int i;
            for (i = 1; i <= length; i++) {
                printf("%c", buf[i]);
            }
        }

        sleep(1);
    }

    disable_spi();
    return 0;
}
