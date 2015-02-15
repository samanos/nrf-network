#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <stdint.h>

void tx_payload();
uint8_t max_retransmits_reached();
uint8_t tx_data_sent();
void clean_max_rt_int();

void send_payload();

#endif
