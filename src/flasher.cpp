#include <Arduino.h>
#include "flasher.h"

    // Constructor - creates a Flasher
    // and initializes the member variables and state
    Flasher::Flasher(int pin, long on, long off)
    {
        ledPin = pin;
        pinMode(ledPin, OUTPUT);

        OnTime = on;
        OffTime = off;

        ledState = LOW;
        previousMillis = 0;
    }

   void Flasher::Update(unsigned long currentMicros)
    {
        if ((ledState == HIGH) && (currentMicros - previousMillis >= OnTime))
        {
            ledState = LOW;                 // Turn it off
            previousMillis = currentMicros; // Remember the time
            digitalWrite(ledPin, ledState); // Update the actual LED
        }
        else if ((ledState == LOW) && (currentMicros - previousMillis >= OffTime))
        {
            ledState = HIGH;                // turn it on
            previousMillis = currentMicros; // Remember the time
            digitalWrite(ledPin, ledState); // Update the actual LED
        }
    }

