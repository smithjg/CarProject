#include <Arduino.h>
#ifndef RadioInputSample_H
#define RadioInputSample_H
typedef enum  {
    waitingToStart,
    pwmHigh,
    pwmLow
}state_t;

typedef struct 
{
    state_t state;
    unsigned long microsResult;
}result_t;

uint8_t
ReadRadioPWM(unsigned long micros);

void
 SetResult(void);

unsigned long ExportPWMResults(void);

 void ReportCurrentState(void);
 #endif
