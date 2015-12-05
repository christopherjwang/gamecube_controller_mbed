#include "Gamecube.h"

//================================================================================
// Gamecube
//================================================================================

extern "C" int my_asm(uint32_t *buff, uint8_t len);
extern "C" int gc_asm_read(uint32_t *buff, uint8_t len);

Serial pc(USBTX, USBRX); // tx, rx
DigitalIn my_in(p10);

void reverse_array(uint32_t *arr, uint8_t count) {
    int temp, i;
    for (i = 0; i < count/2; ++i) {
        temp = arr[count-i-1];
        arr[count-i-1] = arr[i];
        arr[i] = temp;
    }
}


Gamecube::Gamecube(PinName _data_line)
    :data_line(_data_line)
{
    
}
void Gamecube::gc_asm(uint32_t* buff, uint8_t len) {
    data_line.output();
    reverse_array(buff, len);
    my_asm(buff, len);
}

void Gamecube::gc_read(uint8_t len) {
    //data_line.input();
    uint32_t data[len*8];
    for (int i=0; i < len*8; i++) data[i]=0;
    gc_asm_read(data, len*8);
    for (int i=0; i < len*8; i++) {
        pc.printf("%d", data[i]);    
    }
    pc.printf("\n\r");
}

void Gamecube::gc_send(uint32_t* buff, uint8_t len) {
    __disable_irq();
    uint8_t i;
    data_line.output();
    for (i=0; i < len; i++) {
        if (buff[i]) {
            data_line.write(0);
            wait_us(.6);
            data_line.write(1);
            wait_us(2.6);
            data_line.write(1);
        } else {
            data_line.write(0);
            wait_us(2.6);
            data_line.write(1);
            wait_us(.6);
            data_line.write(1);
        }
    }
    __enable_irq();
}