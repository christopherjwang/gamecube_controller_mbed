#include "Gamecube.h"

//================================================================================
// Gamecube
//================================================================================

extern "C" int my_asm(uint32_t *buff, uint8_t len);
extern "C" int gc_asm_read(uint32_t *buff, uint8_t len, uint32_t *read_buff, uint8_t read_buff_len);
extern "C" int gc_asm_write_read(uint32_t *buff, uint8_t len, uint32_t *read_buff, uint8_t read_buff_len);


Serial pc(USBTX, USBRX); // tx, rx
DigitalIn my_in(p10);
DigitalOut p(p19);


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
    reverse_array(buff, len);
    my_asm(buff, len);
}

void Gamecube::gc_read(uint8_t len) {
    uint32_t data[len*8];
    for (int i=0; i < len*8; i++) data[i]=0;
    gc_asm_read(NULL, 0, data, len*8);
    for (int i=0; i < len*8; i++) {
        pc.printf("%d", data[i]);    
    }
    pc.printf("\n\r");
}


void Gamecube::update(void) {
    int num_bits = 8 * 8;
    uint32_t data[num_bits];
    for (int i=0; i < num_bits; i++) data[i]=0;
    //g.gc_write_read(start_sequence, 8, data, num_bits);

    //g.gc_asm(start_sequence, 8);
    uint32_t status_sequence[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0};
    __disable_irq();
    gc_write_read(status_sequence, 24, data, num_bits);
    __enable_irq();
    
    // data[0] data[1] data[2] are not used
    START   = data[3] == 1;
    Y       = data[4] == 1;
    X       = data[5] == 1;
    B       = data[6] == 1;
    A       = data[7] == 1;
    
    //data[8] is always 1
    L       = data[9] == 1;
    R       = data[10] == 1;
    Z       = data[11] == 1;
    D_UP    = data[12] == 1;
    D_DOWN  = data[13] == 1;
    D_RIGHT = data[14] == 1;
    D_LEFT  = data[15] == 1;
    
    JOYSTICK_X = 0;
    JOYSTICK_Y = 0;
    C_STICK_X = 0;
    C_STICK_Y = 0;
    for (int i=0; i < 8; i++) {
        JOYSTICK_X <<= 1;
        JOYSTICK_X |= data[i+16];
        JOYSTICK_Y <<= 1;
        JOYSTICK_Y |= data[i+24];
        
        C_STICK_X <<= 1;
        C_STICK_X |= data[i+32];
        C_STICK_Y <<= 1;
        C_STICK_Y |= data[i+40];
        
    }
    
    for (int i=0; i < num_bits; i++) {
        pc.printf("%d", data[i]!=0);
        if (i>0 && (i+1)%8 == 0) {
            pc.printf(" ");    
        }
    }
    pc.printf("\n\r");
}

void Gamecube::gc_write_read(uint32_t* wbuff, uint8_t wbuff_len, uint32_t *rbuff, uint8_t rbuff_len) {
    gc_asm_write_read(wbuff, wbuff_len, rbuff, rbuff_len);
}

void Gamecube::gc_send(uint32_t* buff, uint8_t len) {
    __disable_irq();
    uint8_t i;

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