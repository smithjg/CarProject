#ifndef Flasher_h
#define Flasher_h
#include "Arduino.h"

class Flasher
{
private:
    int ledPin;            // the number of the LED pin
    unsigned long OnTime;  // milliseconds of on-time
    unsigned long OffTime; // milliseconds of off-time
    // These maintain the current state
    int ledState;                 // ledState used to set the LED
    unsigned long previousMillis; // will store last time LED was updated

public:
    Flasher(int pin, long on, long off);
    void Update(unsigned long currentMicros);
};
#endif
