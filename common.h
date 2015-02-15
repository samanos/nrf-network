#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

void common_config();
void ptx();
void prx();
void prx_addr();
void receiver_addr();

void power_up();
void power_down();

uint8_t rx_data_ready();
uint8_t get_rx_data();

void flush_tx();
void flush_rx();
void clean_int_flags();
void clean_up();

#endif
