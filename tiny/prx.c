#include "softuart.h"
#include "../receiver.h"

// Forward declaration with 'noreturn' attribute
void main() __attribute__ ((noreturn));

void main() {
    uartInit();
    receiver_loop();
}
