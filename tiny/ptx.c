#include "softuart.h"
#include "../platform.h"
#include "../transmitter.h"

// Forward declaration with 'noreturn' attribute
void main() __attribute__ ((noreturn));

void main() {
    uartInit();
    enable_transmitter();

    while (1) {
        uint8_t addr = 0xA1;
        uint8_t payload[8] = { 0xF0, 0x1D, 0xAB, 0x1E, 0x5C, 0xA1, 0xAB, 0x1E };
        uint8_t reply[32];
        int8_t length = send_payload(&addr, 1, payload, 8, reply);
        if (length > 0) {
          print_buf(reply, length);
        }
        delay_ms(2000);
    }
}
