#include "softuart.h"
#include "../platform.h"
#include "../transmitter.h"

// Forward declaration with 'noreturn' attribute
void main() __attribute__ ((noreturn));

void main() {
    uartInit();

    while (1) {
        send_payload();
        delay_ms(2000);
    }
}
