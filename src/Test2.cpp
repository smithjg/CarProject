#include <Arduino.h>
#include "Test2.h"


unsigned int externalVariable = 100;

static unsigned int externalVariable2 = 100;
unsigned int ReturnTest2Variable(void)
{
    return externalVariable2;
}

void SetTest2Variable(unsigned int inVar)
{
    externalVariable2 = inVar;
}

void PrintTest2Variables(void)
{
    Serial.print(" Publicly visible variable is : ");
    Serial.print(externalVariable, DEC);
    Serial.print(" Invisible variable is : ");
    Serial.print(externalVariable2, DEC);
    Serial.print('\n');
}
