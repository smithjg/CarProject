// Jeffs attempt to make a non blocking polling PWM input 
// c to start with - make c++ later with a constructor 
#include <Arduino.h>
#include "RadioInputSample.h"
#include <limits.h>

#define result_t_Default {(state_t)waitingToStart,(unsigned long)0}
#define NUM_ELEMENTS_IN_CURRENT_RESULT 50
static uint32_t radioSignalMemory = 0;
static unsigned long microsMemory = 0;
static unsigned long microsResult = 0;
static uint8_t pinPwmInRadio = 5; // update in the constructor if it works
static bool setresult = false;
static uint8_t max = 0;
static state_t state = waitingToStart;
static result_t current_result[NUM_ELEMENTS_IN_CURRENT_RESULT] = {result_t_Default,result_t_Default,result_t_Default,result_t_Default,result_t_Default};
static const uint8_t current_result_ptr_max = NUM_ELEMENTS_IN_CURRENT_RESULT; // (sizeof current_result)/(sizeof current_result[0]);
static uint8_t current_result_ptr = 0;

unsigned long test_cumulative =0;
// Long brute force calc - 
static unsigned long cumulative_moving_average(uint8_t max)
{
    int i = 0;
    unsigned long micros_cumulative = 0;

    // Make sure that the max number of values does not exceed that of the possible storage 
    if (max >= NUM_ELEMENTS_IN_CURRENT_RESULT)
    {
        max = NUM_ELEMENTS_IN_CURRENT_RESULT;
    }

    for(;i<max;i++)
    {
        if((ULONG_MAX-micros_cumulative)>current_result[i].microsResult)
        {// if we are not going to overload the capacity of the field add the numbers 
            micros_cumulative = micros_cumulative+current_result[i].microsResult;
        }
        else
        {// this route only if we overload 
            --i;
            break;
        }
    }
    // return the average over the last max values     
    return micros_cumulative/i;
}

static uint8_t store_current_result(state_t stateIN, unsigned long microsResultIN)
{
    current_result[current_result_ptr].state = stateIN;
    current_result[current_result_ptr].microsResult = microsResultIN;
    current_result_ptr++;

    // during the short time it takes to fill the circular buffer use the number we have collected
    if(max<NUM_ELEMENTS_IN_CURRENT_RESULT)max++;
    
    if (current_result_ptr == NUM_ELEMENTS_IN_CURRENT_RESULT)
    {// wrap the pointer if appropriate 
        current_result_ptr = 0;
    }
    return current_result_ptr;
}

uint8_t
ReadRadioPWM1(unsigned long micros)
{
    int radioSignal = digitalRead(pinPwmInRadio);
    return (true==radioSignal)?true:false;
}

uint8_t
ReadRadioPWM(unsigned long micros)
{
    uint8_t retVal=0;
    int radioSignal = digitalRead(pinPwmInRadio);

    switch (state)
    {
    case waitingToStart:
        if (radioSignal == true)
        {
            state = pwmHigh;
            radioSignalMemory = micros;
        }
        break;
    case pwmHigh:
        if (radioSignal == false)  // a high to low transition - end of the P Width period 
        {
            state = pwmLow;
            microsResult = micros - radioSignalMemory;  // capture the time in the high state 
            store_current_result(state, microsResult);  // storing state is a bit pointless 
            if(setresult==true) ReportCurrentState();   // debug print out 
        }
        break;
    case pwmLow:
        if ((micros - microsMemory) > 3000)
            state = waitingToStart;
        break;
    }
    return retVal;
}

void
 SetResult(void)
{
    setresult=true;
}

unsigned long ExportPWMResults(void)
{
    test_cumulative = cumulative_moving_average(max);
    return test_cumulative;
}

// debug function - write the data out when triggered in line with the 
// Main loop 
// Called from within the pwm input state machine but only enabled to report 
// by an external signal from the main loop 
void ReportCurrentState(void)
{
    char buffer[500];
    sprintf(buffer, "\n state: %d MeasuredPWM Time %lu : %lu \r\n", state, ExportPWMResults(),test_cumulative);
    Serial.print(buffer);
    setresult = false; // 
}
