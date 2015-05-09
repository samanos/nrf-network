#include "platform.h"
#include "common.h"

/**
 * Configure ptx specific registers.
 */
static void ptx()
{
    // read CONFIG register
    uint8_t buf[2] = { 0, 0 };
    spi_transfern(buf, 2);

    buf[0] = 0b00100000; // write to CONFIG
    buf[1] |= 0 << 0; // PRX
    spi_transfern(buf, 2);
}

/**
 * Transmit payload.
 */
static void tx_payload(uint8_t *payload, uint8_t payload_len)
{
    // W_TX_PAYLOAD command
    uint8_t buf[9] = { 0b10100000, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t i;
    for (i = 0; i < 8 && i < payload_len; i++) {
      buf[i + 1] = payload[i];
    }
    spi_transfern(buf, i + 1);
}

/**
 * Check if max retransmits has been reached.
 */
static uint8_t max_retransmits_reached()
{
    uint8_t status = spi_transfer((uint8_t) 0xFF);
    // MAX_RT is bit 4 in status register
    return (status >> 4) & 1;
}

/**
* Check if data has been sent, which means that ACK has been received with or without payload.
*/
static uint8_t tx_data_sent()
{
    uint8_t status = spi_transfer((uint8_t) 0xFF);
    // TX_DS is bit 5 in status register
    return (status >> 5) & 1;
}

void enable_transmitter()
{
    enable_spi();
    common_config();
    ptx();

    ce_high();
    power_up();

    clean_up();
}

int8_t send_payload(uint8_t *addr, uint8_t addr_len, uint8_t *payload, uint8_t payload_len, uint8_t *reply)
{
    receiver_addr(addr, addr_len);
    tx_payload(payload, payload_len);
    while (!tx_data_sent() && !max_retransmits_reached()) {}

    int8_t length = -1;
    if (max_retransmits_reached()) {
        println("Unable to send data.");
    }
    else if (!rx_data_ready()) {
        println("Empty ack this time!");
    }
    else {
        println("Data ready, reading...");

        uint8_t status = spi_transfer((uint8_t) 0xFF);
        print("Status ");
        print_buf(&status, 1);

        length = get_rx_data(reply);
    }

    clean_up();
    return length;
}
