#include <Arduino.h>

// this is an abstraction for clarity and only for debugging at his point
// uses globals - will tidy 

extern unsigned long targetMicros;
extern bool direction;
extern uint16_t countTarget;
extern const uint16_t countLimit;


// Interrupt driven elements //////////////////////////////////
/// Function to test the use of interrupts on the serial in line
/// This is a Arduino thing I guess
bool serialEventDirect(int character)
{
  bool ret_val = true;

  if (character == 'f') // f567
  {
    countTarget = 1;
  }
  else if (character == 's') // s
  {
    countTarget = 40;
  }
  if (character == '-')
  {
    if(countTarget > 0) countTarget--;
  }
  else if (character == 61)
  {
    if(++countTarget > countLimit) countTarget = countLimit;
  }
  else{
    ret_val = false;
  }
  return ret_val; 
}

/// Interrupt driven elements //////////////////////////////////
/// Function to test the use of interrupts on the serial in line
/// This is a Arduino thing I guess
bool serialEventMeasured(int character)
{  
  bool ret_val = true;

  if (character == 'f') // f567
  {
    targetMicros = 2200;//80
  }
  else if (character == 's') // s
  {
    targetMicros = 5000;// 10
  }
  else if (character == 'a') // s
  {
    targetMicros = 2700;//20
  }
   else if (character == 'b') // s
  {
    targetMicros = 8100;//4a
  }
  else
  {
    ret_val = false;
  }
  return ret_val;
}

// Interrupt driven elements //////////////////////////////////
/// Function to test the use of interrupts on the serial in line
/// This is a Arduino thing I guess
void serialEvent()
{
  int eric = Serial.read();
  Serial.print(eric, DEC);

  if (true == serialEventMeasured(eric))
  {

  }
  else if (eric == 'd')
  {
    if (direction == HIGH)
    {
      direction = LOW;
    }
    else
    {
      direction = HIGH;
    }
  }
}

