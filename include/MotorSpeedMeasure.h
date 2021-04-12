#ifndef MotorSpeedMeasure_h
#define MotorSpeedMeasure_h

#include <Arduino.h>

void pin_ISR();
unsigned long GetMeasuredMicros(void);
uint16_t GetNumTimesSpeedMeasured(void);
#endif