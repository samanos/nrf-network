#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <stdint.h>

/**
 * Initialize all required HW and configuration for transmitting.
 */
void enable_transmitter();

/**
 * Configure and send predefined payload,
 */
int8_t send_payload(uint8_t *addr, uint8_t addr_len, uint8_t *payload, uint8_t payload_len, uint8_t *reply);

#endif
