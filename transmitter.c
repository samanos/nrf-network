#include "platform.h"
#include "common.h"

void tx_payload()
{
    // W_TX_PAYLOAD command
    uint8_t buf[11] = { 0b10100000, 'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd' };
    spi_transfern(buf, 11);
}

uint8_t max_retransmits_reached()
{
    uint8_t status = spi_transfer((uint8_t) 0xFF);
    // MAX_RT is bit 4 in status register
    return (status >> 4) & 1;
}

uint8_t tx_data_sent()
{
    uint8_t status = spi_transfer((uint8_t) 0xFF);
    // TX_DS is bit 5 in status register
    return (status >> 5) & 1;
}

void clean_max_rt_int()
{
    uint8_t status = spi_transfer((uint8_t) 0xFF);
    status |= 1 << 4;

    // write to STATUS register
    char buf[2] = { 0b00100111, status };
    spi_transfern(buf, 2);
}

void send_payload()
{
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
        println("Unable to send data.");
    }
    else if (!rx_data_ready()) {
        println("Empty ack this time!");
    }
    else {
        print("Data ready, reading: ");

        uint8_t status = spi_transfer((uint8_t) 0xFF);
        print("Status ");
        print_buf(&status, 1);

        char buf[32];
        uint8_t length = get_rx_data(buf);
        print_buf(buf, length + 1);
    }

    clean_up();
    power_down();
    disable_spi();
}