#ifndef _ENCODER_ARDUINO_H_
#define _ENCODER_ARDUINO_H_
#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

void encoderISR();
void encoderSetup(uint8_T pinA, uint8_T pinB);
int readEncoder();

#ifdef __cplusplus
}
#endif
#endif //_ENCODER_ARDUINO_H_