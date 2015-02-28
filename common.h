#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

/**
 * Configure NRF24 registers that are common for prx and ptr.
 */
void common_config();

/**
 * Set prx listen address.
 */
void prx_addr(uint8_t *addr, uint8_t addr_len);

/**
 * Set receiver address and listen address for acks.
 */
void receiver_addr(uint8_t *addr, uint8_t addr_len);

/**
 * Go to POWER UP state.
 */
void power_up();

/**
 * Got to POWER DOWN state.
 */
void power_down();

/**
 * Check if data is ready in the RX FIFO by querying which data pipe has data available.
 */
uint8_t rx_data_ready();

/**
 * Clock in received data from RX FIFO.
 */
int8_t get_rx_data(uint8_t *rx_data);

/**
 * Flush TX FIFO. In prx this holds ACK payloads.
 */
void flush_tx();

/**
 * Flush RX FIFO.
 */
void flush_rx();

/**
 * Clear interrupt flags.
 */
void clean_int_flags();

/**
 * Flush TX and RX FIFOs and clear interrupt flags.
 */
void clean_up();

#endif
