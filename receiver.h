#ifndef RECEIVER_H
#define RECEIVER_H

#include <stdint.h>

/**
 * Initialize all required HW and configuration for receiving.
 */
void enable_receiver(uint8_t *addr, uint8_t addr_len);

/**
 * A loop where receiver puts ACK payloads and handles received messages.
 */
void receiver_loop() __attribute__ ((noreturn));

#endif
