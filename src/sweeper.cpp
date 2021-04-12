#include <Arduino.h>
#include "Sweeper.h"
#include <servo.h>

/// Constructor giving ability to set the update interval
Sweeper::Sweeper(int intv) : increment((int)0x1),updateInterval(intv) {}

// called with no parameters default to 25 for update interval 
Sweeper::Sweeper() : Sweeper((int)25){};

/// Allows the Attaching of a servo object to the pin that the servo is connected to 
bool Sweeper::Attach(int pin)
{
    bool retval = false;
    if (digitalPinHasPWM(pin))
    {
        servo.attach(pin); // attaches the servo on designated pin to the servo object
        retval = true;
    }
  
    return retval;
}

void Sweeper::Detach()
{
    servo.detach();
}

// Causes the chnge of position of the contained servo
void Sweeper::Update(unsigned long currentMicros)
{
    if ((currentMicros - lastUpdate) > updateInterval) // time to update
    {
        lastUpdate = millis();
        position += increment;
        servo.write(position);
        if ((position >= 180) || (position <= 0)) // end of sweep
        {
            // reverse direction
            increment = -increment;
        }
    }
}
