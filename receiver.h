#ifndef RECEIVER_H
#define RECEIVER_H

#include <stdint.h>

/**
 * A loop where receiver puts ACK payloads and handles received messages.
 */
void receiver_loop(uint8_t *addr, uint8_t addr_len) __attribute__ ((noreturn));

#endif
