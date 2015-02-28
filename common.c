#include <string.h>

#include "platform.h"

void common_config()
{
    // CONFIG
    // no interrupts, crc 2 bytes
    uint8_t buf[2] = { 0b00100000, 0b01111100 };
    spi_transfern(buf, 2);

    // EN_AA
    // autoack only on data pipe 0
    buf[0] = 0b00100001; buf[1] = 0b00000001;
    spi_transfern(buf, 2);

    // EN_RXADDR
    // enable only data pipe 0
    buf[0] = 0b00100010; buf[1] = 0b00000001;
    spi_transfern(buf, 2);

    // RF_CH
    // set channel to 0001000
    buf[0] = 0b00100101; buf[1] = 0b00001000;
    spi_transfern(buf, 2);

    // DYNPD
    // enable dynamic payload length for data pipe 0
    buf[0] = 0b00111100; buf[1] = 0b00000001;
    spi_transfern(buf, 2);

    // FEATURE
    // enable dyn payload length, enable payload with ack, enable no_ack command
    buf[0] = 0b00111101; buf[1] = 0b00000111;
    spi_transfern(buf, 2);
}

void prx_addr(uint8_t *addr, uint8_t addr_len)
{
    // RX_ADDR_P0
    // receive address of data pipe 0
    uint8_t buf[6] = { 0b00101010, 0xAF, 0xDE, 0xAD, 0xBE, 0xEF };
    for (uint8_t i = 0; i < 5 && i < addr_len; i++) {
      buf[i + 1] = addr[i];
    }
    spi_transfern(buf, 6);
}

void receiver_addr(uint8_t *addr, uint8_t addr_len)
{
    // TX_ADDR
    // receiver address
    uint8_t buf[6] = { 0b00110000, 0xAF, 0xDE, 0xAD, 0xBE, 0xEF };
    for (uint8_t i = 0; i < 5 && i < addr_len; i++) {
      buf[i + 1] = addr[i];
    }
    spi_transfern(buf, 6);

    // need to set receive address for data pipe 0 for acks
    prx_addr(addr, addr_len);
}

void power_up()
{
    // read CONFIG register
    uint8_t buf[2] = { 0, 0 };
    spi_transfern(buf, 2);

    buf[0] = 0b00100000; // write to CONFIG
    buf[1] |= 1 << 1; // PWR_UP = 1
    spi_transfern(buf, 2);
}

void power_down()
{
    // read CONFIG register
    uint8_t buf[2] = { 0, 0 };
    spi_transfern(buf, 2);

    buf[0] = 0b00100000; // write to CONFIG
    buf[1] &= ~(0 << 1); // PWR_UP = 0
    spi_transfern(buf, 2);
}

uint8_t rx_data_ready()
{
    uint8_t status = spi_transfer(0xFF);
    // RX_P_NO is bits 3:1 in status register
    return ((status >> 1) & 0b00000111) != 0b00000111;
}

int8_t get_rx_data(uint8_t *rx_data)
{
    // R_RX_PL_WID command
    uint8_t buf[33] = { 0b01100000, 0 };
    spi_transfern(buf, 2);

    int8_t length = buf[1];
    if (length <= 32) {
      memset(buf, 0, sizeof(buf));
      // R_RX_PAYLOAD command
      buf[0] = 0b01100001;
      spi_transfern(buf, length + 1); // one for status byte

      // at the first position of buf is a status byte
      for (uint8_t i = 0; i < length; i++) {
        rx_data[i] = buf[i + 1];
      }
    }
    else {
      length = -1;
    }

    return length;
}

void flush_tx()
{
    // FLUSH_TX command
    spi_transfer(0b11100001);
}

void flush_rx()
{
    // FLUSH_RX command
    spi_transfer(0b11100010);
}

void clean_int_flags()
{
    // STATUS register
    // RX_DR bit6, TX_DS bit5, MAX_RT bit4
    uint8_t buf[2] = { 0b00100111, 0b01110000 };
    spi_transfern(buf, 2);
}

void clean_up()
{
    flush_tx();
    flush_rx();
    clean_int_flags();
}
