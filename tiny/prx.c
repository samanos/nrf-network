#include "softuart.h"
#include "../receiver.h"

// Forward declaration with 'noreturn' attribute
void main() __attribute__ ((noreturn));

void main() {
    uartInit();
    uint8_t addr = 0xA1;
    enable_receiver(&addr, 1);
    receiver_loop();
}
