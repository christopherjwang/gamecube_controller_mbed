#include "mbed.h"
#include "Gamecube.h"
DigitalOut myled(LED1);

// declare LED outputs – let C set them up as output bits
DigitalOut myled1(LED1);
DigitalOut myled4(LED4);

int main() {
    Gamecube g(p9);
    uint32_t c[] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
    int value = 0;
    
//    // loop forever
    uint32_t a[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};//{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
//    uint32_t b[] = {1};//{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
//    
//    myled4=0;
//    while(1) {
//        
//        myled1=1;
//        //g.gc_asm(c, 24);
//        g.gc_asm(a, 1);
//        wait(1);
//        myled4 = !myled4;
//        g.gc_asm(b, 1);
//        wait(1);
//        myled1=0;
//    }

    
    while(1) {
        g.gc_asm(c, 24);
        //g.gc_send(c, 24);
    }
}
