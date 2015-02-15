#ifndef RECEIVER_H
#define RECEIVER_H

void clean_rx_dr_int();
void receiver_loop() __attribute__ ((noreturn));

#endif
