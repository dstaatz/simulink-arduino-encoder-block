#include <Arduino.h>
#include "encoder_arduino.h"

#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

volatile int position;
volatile uint32_t * pinA_register;
volatile uint32_t * pinB_register;
uint32_t pinA_bitmask;
uint32_t pinB_bitmask;

void encoderISR() {
    static uint8_t state = 0;
    uint8_t s = state & 3;
    if (DIRECT_PIN_READ(pinA_register, pinA_bitmask)) s |= 4;
    if (DIRECT_PIN_READ(pinB_register, pinB_bitmask)) s |= 8;
    switch (s) {
        case 0: case 5: case 10: case 15:
            break;
        case 1: case 7: case 8: case 14:
            position++; break;
        case 2: case 4: case 11: case 13:
            position--; break;
        case 3: case 12:
            position += 2; break;
        default:
            position -= 2; break;
    }
    state = (s >> 2);
}

// Create an Encoder object and save it to the pointer
extern "C" void encoderSetup(uint8_T pinA, uint8_T pinB)
{ 
    pinMode(pinA, INPUT_PULLUP);
	pinMode(pinB, INPUT_PULLUP);

    position = 0;
    pinA_register = portInputRegister(digitalPinToPort(pinA));
    pinB_register = portInputRegister(digitalPinToPort(pinB));
    pinA_bitmask = digitalPinToBitMask(pinA);
    pinB_bitmask = digitalPinToBitMask(pinB);

    attachInterrupt(digitalPinToInterrupt(pinA), encoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pinB), encoderISR, CHANGE);
} 

// Read the position relative to the old position
// and reset the position to zero
extern "C" int readEncoder() 
{ 
    int currentPosition = position;
    position = 0;
    return currentPosition;
}