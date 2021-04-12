#include <Arduino.h>
#include <stdio.h>
#include "Test2.h"
#include "flasher.h"
#include "InterruptTimerAction.h"
#include "RadioInputSample.h"
#include "CommandMapping.h"
#include "MotorSpeedMeasure.h"


uint16_t adjustTheSpeed(unsigned long measuredMicros);

#define __ASSERT_USE_STDERR

#include <assert.h>

///////////////////////////////////////////////////////////////////////////
// Set up some parameters 
const uint16_t countLimit = 80;
uint16_t countTarget = 10 ;//(countLimit/10)*FB_GAIN;// initial value 
bool direction = HIGH;
uint16_t numbreroftimesSpeedmeasuredMax = 0;
unsigned long targetMicros = 9100;
uint32_t watchdog = 100;
unsigned long measuredPWMFromRadio =0;
bool MotorTick = false;// Flag to be set each time outout period to Motor is achieved - 20Ms 

///////////////////////////////////////////////////////////////////////////
// Set up the pins we attach to peripherials 
const int isrPin = 2;  
Flasher led3(LED_BUILTIN, 500, 500);
const uint8_t outputEna = 4;
const uint8_t output1 = 7;
const uint8_t output2 = 8;
const uint8_t updateRateIndicatorOutput = 9;
const uint8_t pinPwmInRadio = 5;


#define  DiagTick {static uint8_t outputDiag = LOW;outputDiag = !outputDiag;digitalWrite(updateRateIndicatorOutput, outputDiag);}

void settargetMicrosFromTX(unsigned long pwmRead)
{
  targetMicros = map_command_value_from_pwm_in(pwmRead);
}

// Set up the ASSERT handler 
// handle diagnostic informations given by assertion and abort program execution:
void __assert(const char *__func, const char *__file, int __lineno, const char *__sexp) {
    // transmit diagnostic informations through serial link. 
    Serial.println(__func);
    Serial.println(__file);
    Serial.println(__lineno, DEC);
    Serial.println(__sexp);
    Serial.flush();
    // abort program execution.
    abort();
}

///////////////////////////////////////////////////////////////////

/// 
/// 
uint16_t calculateTheSpeedAdjust (unsigned long measuredMicros)
{
    return (measuredMicros > targetMicros) ?measuredMicros-targetMicros:targetMicros-measuredMicros;
}

// the count target is actually FB_GAIN greater than the count limit 
// if is divided by FB_GAIN when used 
// This allows the slower increment and decrement ??
uint8_t incrementCountTarget(void)
{
  countTarget++;
  
  if ( countTarget >= countLimit)
  {
    countTarget = countLimit;
  }
  return countTarget;
}

uint8_t decrementCountTarget(void)
{
  if (countTarget > 1)
  {
    countTarget = countTarget - 1;
  }
  return countTarget;
}

/// This actually sets the speed
/// Needs to be used in a feedback loop
// The bigger countTarget The faster we go
// The faster we go the smaller is measuredMillis
uint16_t adjustTheSpeed(unsigned long measuredMicros)
{
  if (measuredMicros > 0)
  {
    if (measuredMicros > targetMicros) // we need to go faster increase countTarget
    {
      (uint8_t) incrementCountTarget();
    }

    if (measuredMicros < targetMicros)
    { // we need to go slower decrease countTarget
      (uint8_t) decrementCountTarget();
    }
  }
  return countTarget;
}

uint_fast16_t do_half_second_actions(void)
{
  char buffer[500];
  sprintf(buffer, "Measured Micros:%lu targetMicros:%lu CountTarget:%d Num Measurements %d.\n\r", GetMeasuredMicros(),targetMicros,countTarget,numbreroftimesSpeedmeasuredMax);
  Serial.print(buffer);
  
  SetResult(); // other elements sync
  return (uint16_t)0;
}

//////////////////////////////////////////////////////////////////
// Timer Actions (so as to not need stuff in the loop)
// Set up a set of actions to be performed on a timer interrupt
// Sim to be 0.5 Ms
void timerActions(unsigned long now)
{
  static uint16_t counter = 0;
  static uint16_t counterInner = 0;
  static uint16_t counter500 = 0;
  uint8_t output = LOW;
  uint8_t outputActive = output1;
  uint8_t outputInActive = output2;

  //countTarget Increases causes more ON Time in the period
  if (++counter500 > 1000)
  {// can do this in several places - here I link it to the timer 
    counter500 = do_half_second_actions();
  }

  //countTarget Increases causes more ON Time in the period
  if (++counter == countLimit)
  {
    counter = 0;
    counterInner++;
    // more diagnostics
    numbreroftimesSpeedmeasuredMax = GetNumTimesSpeedMeasured();
    if (counterInner == 2)
    {
      (uint16_t) adjustTheSpeed(GetMeasuredMicros());
      counterInner = 0;
    }

    MotorTick = true; //sync  with the other elements that are interested in the motor update
  }

  // handle the PWM 0-100%
  output = (counter >= countTarget) ? LOW : HIGH;

  if (direction == HIGH) 
  {
    outputActive = output2;
    outputInActive = output1;
  }

  // Activate the PWM
  digitalWrite(outputActive, HIGH);  // control with the variable on time
  digitalWrite(outputInActive, LOW); // keep turned off
  digitalWrite(outputEna, output);
}

//////////////////////////////////////////////////////////////////
/// The Setup element 
///
void setup()
{
  Serial.begin(9600); // open the serial port at 9600 bps:
  initTimer(timerActions);

  pinMode(isrPin, INPUT);
  pinMode(pinPwmInRadio, INPUT); // set as an input only - the sampling is done locally 
  digitalWrite(isrPin, HIGH); // enable internal pull ups 

  // as only specific pins can support the ISR and the command needs the isr number not the pin number
  // translate using a built in function appropriate to the board which will set the pin appropriate to the Require ISR
  attachInterrupt(digitalPinToInterrupt(isrPin), pin_ISR, CHANGE); // Rasing or Lowering or Change

  // Three output pins at the moment - one for each direction of rotation and one for the enable
  pinMode(output2, OUTPUT);
  pinMode(output1, OUTPUT);
  pinMode(outputEna, OUTPUT);
  // set up a digital output for timing with an oscillascope 
  pinMode(updateRateIndicatorOutput, OUTPUT);
}

//////////////////////////////////////////////////////////////////////////////
// Runs at about 10u Seconds 
void loop()
{
  (void)ReadRadioPWM(micros());

  if (true == MotorTick)
  { // this ensures it runs each time the motor output is updated - 20ms
    //measuredPWMFromRadio = ExportPWMResults(); // get filtered Command PWM values
    //led3.Update(millis());                     // comfort message
    MotorTick = false;
  }
}


