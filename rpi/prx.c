#include "../receiver.h"

int main(int argc, char **argv)
{
    uint8_t addr = 0xA1;
    receiver_loop(&addr, 1);
}
