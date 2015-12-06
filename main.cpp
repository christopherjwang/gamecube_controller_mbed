#include "mbed.h"
#include "Gamecube.h"

Serial pc1(USBTX, USBRX); // tx, rx

int main() {
    Gamecube g(p9);
    pc1.printf("BEGIN\n");

    while(1) {
        
        g.update();
        pc1.printf("A: %d \n\rB: %d \n\rX: %d \n\rY: %d \n\rL: %d \n\rR: %d \n\rZ: %d \n\rSTART: %d \n\rD_UP: %d \n\rD_LEFT: %d \n\rD_DOWN: %d \n\rD_RIGHT: %d \n\rJOYSTICK_X: %d \n\rJOYSTICK_Y: %d \n\rC_STICK_X: %d \n\rC_STICK_Y: %d \n\r", g.A, g.B, g.X, g.Y, g.L, g.R, g.Z, g.START, g.D_UP, g.D_LEFT, g.D_DOWN, g.D_RIGHT, g.JOYSTICK_X, g.JOYSTICK_Y, g.C_STICK_X, g.C_STICK_Y);
        wait(1);
    }
}
