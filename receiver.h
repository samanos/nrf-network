#ifndef RECEIVER_H
#define RECEIVER_H

/**
 * Configure prx specific registers.
 */
void prx();

/**
 * Clean RX Data Ready interrupt flag.
 */
void clean_rx_dr_int();

/**
 * A loop where receiver puts ACK payloads and handles received messages.
 */
void receiver_loop() __attribute__ ((noreturn));

#endif
