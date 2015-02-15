#include "platform.h"
#include "common.h"

void clean_rx_dr_int()
{
    uint8_t status = spi_transfer(0xFF);
    status |= 1 << 6;

    // write to STATUS register
    uint8_t buf[2] = { 0b00100111, status };
    spi_transfern(buf, 2);
}

void receiver_loop()
{
    enable_spi();
    common_config();
    prx();
    prx_addr();

    ce_high();
    power_up();

    while(1) {
        clean_up();

        println("Setting auto ack.");
        // Set autoack payload for data pipe 0
        uint8_t buf[6];
        buf[0] = 0b10101000; buf[1] = 'o'; buf[2] = 'h'; buf[3] = 'i'; buf[4] = '!';
        spi_transfern(buf, 5);

        delay_ms(1000);

        while (rx_data_ready()) {
            print("Data ready, reading: ");
            uint8_t buf[32];
            uint8_t length = get_rx_data(buf);
            print_buf(buf, length + 1);

            clean_rx_dr_int();
        }
    }
}
