#include "mbed.h"
#include "Gamecube.h"

int main() {
    Gamecube g(p9);

    uint32_t start_sequence[] = {0,0,0,0,0,0,0,0};
    __disable_irq();
    while(1) {
        g.gc_asm(start_sequence, 8);
        wait_us(100);
        //g.gc_send(c, 24);
    }
}
