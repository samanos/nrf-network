#ifndef __HARDWARE_H
#define __HARDWARE_H

#include <avr/io.h>

#define UART_ENABLED
#define BAUD_RATE 57600

// single pin UART
#define UART_TX PINB4
#define UART_RX PINB4

#endif /* __HARDWARE_H */
