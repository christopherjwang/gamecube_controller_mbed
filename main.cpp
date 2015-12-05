#include "mbed.h"
#include "Gamecube.h"

Serial pc1(USBTX, USBRX); // tx, rx

int main() {
    Gamecube g(p9);
    pc1.printf("begin!\n");

    uint32_t start_sequence[] = {0,0,0,0,0,0,0,0};
    __disable_irq();
    while(1) {
        g.gc_asm(start_sequence, 8);
        g.gc_read(2);
        wait(1);
        
        int num_bits = 2 * 8;
        uint32_t data[num_bits];
        for (int i=0; i < num_bits; i++) data[i]=0;
        g.gc_write_read(start_sequence, 8, data, num_bits*8);
        for (int i=0; i < num_bits; i++) {
            pc1.printf("%d", data[i]);    
        }
        pc1.printf("\n\r");
        wait(1);
    }
}
