/*
Copyright (c) 2014-2015 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef NINTENDO_H
#define NINTENDO_H

#include <stdint.h>
#include <mbed.h>
//#include <Arduino.h>

//================================================================================
// Settings
//================================================================================

// timeout for the reading function for the pin to go low/high
#define NINTENDO_GAMECUBE_N64_TIMEOUT NINTENDO_GAMECUBE_N64_TIMEOUT_US(28)

//================================================================================
// Definitions
//================================================================================

//#if (F_CPU != 16000000)
//#error This library only supports 16MHz AVRs
//#endif

// timeout for the reading function for the pin to go low/high
#define NINTENDO_GAMECUBE_N64_TIMEOUT_US(uS) (uS * F_CPU / (1000000 * 7))

//================================================================================
// Gamecube
//================================================================================

// gamecube controller device status ids
#define NINTENDO_DEVICE_GC_WIRED 0x0900

// dpad directions
#define NINTENDO_GAMECUBE_DPAD_CENTERED 0
#define NINTENDO_GAMECUBE_DPAD_UP (1 << 3)
#define NINTENDO_GAMECUBE_DPAD_UP_RIGHT (NINTENDO_GAMECUBE_DPAD_UP | NINTENDO_GAMECUBE_DPAD_RIGHT)
#define NINTENDO_GAMECUBE_DPAD_RIGHT (1 << 1)
#define NINTENDO_GAMECUBE_DPAD_DOWN_RIGHT (NINTENDO_GAMECUBE_DPAD_DOWN | NINTENDO_GAMECUBE_DPAD_RIGHT)
#define NINTENDO_GAMECUBE_DPAD_DOWN (1 << 2)
#define NINTENDO_GAMECUBE_DPAD_DOWN_LEFT (NINTENDO_GAMECUBE_DPAD_DOWN | NINTENDO_GAMECUBE_DPAD_LEFT)
#define NINTENDO_GAMECUBE_DPAD_LEFT (1 << 0)
#define NINTENDO_GAMECUBE_DPAD_UP_LEFT (NINTENDO_GAMECUBE_DPAD_UP | NINTENDO_GAMECUBE_DPAD_LEFT)

typedef union{
    // 8 bytes of datareport that we get from the controller
    uint8_t whole8[];
    uint16_t whole16[];
    uint32_t whole32[];

    struct{
        uint8_t buttons0;
        union{
            uint8_t buttons1;
            uint8_t dpad : 4;
        };
    };

    struct {
        // first data byte (bitfields are sorted in LSB order)
        uint8_t a : 1;
        uint8_t b : 1;
        uint8_t x : 1;
        uint8_t y : 1;
        uint8_t start : 1;
        uint8_t high0 : 1;
        uint8_t errlatch : 1;
        uint8_t errstat : 1;

        // second data byte
        uint8_t dleft : 1;
        uint8_t dright : 1;
        uint8_t ddown : 1;
        uint8_t dup : 1;
        uint8_t z : 1;
        uint8_t r : 1;
        uint8_t l : 1;
        uint8_t high1 : 1;

        // 3rd-8th data byte
        uint8_t xAxis;
        uint8_t yAxis;
        uint8_t cxAxis;
        uint8_t cyAxis;
        uint8_t left;
        uint8_t right;
    };
} Gamecube_Data_t;

typedef union{
    // 3 bytes of statusreport that we get from the controller
    uint8_t whole8[];
    uint16_t whole16[];
    struct {
        // device information
        uint16_t device;

        // controller status (only rumble is known)
        uint8_t status0 : 3;
        uint8_t rumble : 1;
        uint8_t status1 : 4;
    };
} Gamecube_Status_t;

class Gamecube{
public:
    //Gamecube(void);
    Gamecube(PinName _data_line);

    bool begin(const uint8_t pin, Gamecube_Status_t &status);
    bool begin(const uint8_t pin);
//    bool end(const uint8_t pin);
//    bool read(const uint8_t pin, Gamecube_Data_t &report, const bool rumble = false); // default no rumble
//    inline void write(void){} // TODO
    void gc_asm(uint32_t* buff, uint8_t len);
    void gc_send(uint32_t* buff, uint8_t len);
    
    void gc_read(uint8_t len);
    void gc_write_read(uint32_t* wbuff, uint8_t wbuff_len, uint32_t *rbuff, uint8_t rbuff_len);

private:
    DigitalOut data_line;
};

//
////================================================================================
//// Function prototypes
////================================================================================
//

// functions to communicate with the gc/n64 controller
//void gc_send(uint8_t* buff, uint8_t len);

//void gc_send(uint8_t* buff, uint8_t len,
//    volatile uint8_t* modePort, volatile uint8_t* outPort, uint8_t bitMask);
//
//uint8_t gc_get(uint8_t* buff, uint8_t len,
//    volatile uint8_t* modePort, volatile uint8_t* outPort, volatile uint8_t * inPort, uint8_t bitMask);
//
//
#endif // include guard