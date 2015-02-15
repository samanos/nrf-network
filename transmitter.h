#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <stdint.h>

/**
 * Configure ptx specific registers.
 */
void ptx();

/**
 * Transmit payload.
 */
void tx_payload();

/**
 * Check if max retransmits has been reached.
 */
uint8_t max_retransmits_reached();

/**
 * Clear max retransmits reached flag.
 */
void clean_max_rt_int();

/**
 * Check if data has been sent, which means that ACK has been received with or without payload.
 */
uint8_t tx_data_sent();

/**
 * Configure and send predefined payload,
 */
void send_payload();

#endif
