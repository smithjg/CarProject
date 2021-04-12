#include <Arduino.h>
#include "MotorSpeedMeasure.h"

//////////////Debug
const unsigned long SMOOTH_FACTOR = 16;
uint16_t numbreroftimesSpeedmeasured = 0;
uint16_t speedmeasuredIndicator = 0;
unsigned long currentMicros = micros();
unsigned long lastMicros = micros();
unsigned long measuredMicros = micros();

unsigned long cumulativeMeasuredMicros = micros();
unsigned long count = 0;
void accumulate(unsigned long in)
{
    //cumulativeMeasuredMicros = ((cumulativeMeasuredMicros*(SMOOTH_FACTOR-1))+in)/SMOOTH_FACTOR; // simple smoothing 
    uint_fast64_t input = (uint_fast64_t)in;
    uint_fast64_t cumulativeMeasuredMicros64 = (((uint_fast64_t)cumulativeMeasuredMicros)*(SMOOTH_FACTOR-1))+input;
    cumulativeMeasuredMicros = (unsigned long)(cumulativeMeasuredMicros64/SMOOTH_FACTOR);
    count++;
}

///////////////////
/// Speedy Interrupt Service routines
// here we work out the time between edges of the hall effect inputs
void pin_ISR()
{
    currentMicros = micros();
    measuredMicros = (currentMicros > lastMicros) ? currentMicros - lastMicros : 0;
    if ((measuredMicros > 100) && (measuredMicros < 30000))
    {
        accumulate(measuredMicros);
        numbreroftimesSpeedmeasured++;
        speedmeasuredIndicator++;
    }

    lastMicros = currentMicros;
}

unsigned long GetMeasuredMicros(void)
{
    return cumulativeMeasuredMicros;
}
unsigned long GetMeasuredMicros1(void)
{
    unsigned long retval = 0;
    if ((count > 8) && (speedmeasuredIndicator!=0))
    {
        retval = cumulativeMeasuredMicros;
        speedmeasuredIndicator = 0;
    }

    return retval;
}

uint16_t GetNumTimesSpeedMeasured(void)
{
    unsigned long ret_val = numbreroftimesSpeedmeasured;
    numbreroftimesSpeedmeasured = 0;
    return ret_val;
}