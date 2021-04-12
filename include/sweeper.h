#ifndef Sweeper_h
#define Sweeper_h

#include <Arduino.h>
#include <Servo.h>

class Sweeper
{
private:
    Servo servo;                 // the servo
    int position;                // current servo position
    int increment;               // increment to move for each interval
    unsigned int updateInterval; // interval between updates
    unsigned long lastUpdate;    // last update of position


public:
    Sweeper();
    Sweeper(int intv);
    bool Attach(int pin);
    void Detach();
    void Update(unsigned long currentMicros);
};
#endif