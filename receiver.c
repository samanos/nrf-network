#include "platform.h"
#include "common.h"

/**
 * Configure prx specific registers.
 */
static void prx()
{
    // read CONFIG register
    uint8_t buf[2] = { 0, 0 };
    spi_transfern(buf, 2);

    buf[0] = 0b00100000; // write to CONFIG
    buf[1] |= 1 << 0; // PRX
    spi_transfern(buf, 2);
}

/**
 * Clean RX Data Ready interrupt flag.
 */
static void clean_rx_dr_int()
{
    uint8_t status = spi_transfer(0xFF);
    status |= 1 << 6;

    // write to STATUS register
    uint8_t buf[2] = { 0b00100111, status };
    spi_transfern(buf, 2);
}

void receiver_loop(uint8_t *addr, uint8_t addr_len)
{
    enable_spi();
    common_config();
    prx();
    prx_addr(addr, addr_len);

    ce_high();
    power_up();

    while(1) {
        clean_up();

        println("Setting auto ack.");
        // Set autoack payload for data pipe 0
        uint8_t buf[4] = { 0b10101000, 0xCA, 0x11, 0xED };
        spi_transfern(buf, 4);

        delay_ms(1000);

        while (rx_data_ready()) {
            print("Data ready, reading: ");
            uint8_t buf[32];
            int8_t length = get_rx_data(buf);
            if (length > 0)
              print_buf(buf, length);

            clean_rx_dr_int();
        }
    }
}
