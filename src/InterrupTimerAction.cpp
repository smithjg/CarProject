#include "InterruptTimerAction.h"

static timerAction_t theActions = nullptr;

// the action to be performed when either the a or B timer occur 
void action(void)
{
  if (nullptr != theActions) {(void) theActions(micros());}
}
// The SERVICE indicates an interrupt service routine and the one specificed is the Timer/Counter0 Output Compare Match A Interrupt
// SIGNAL is the old-school name for an interrupt service routine when using avr-gcc.
// The notation is deprecated on the avr. Please use ISR(VECTOR_NAME) { ... } instead. 
// See http://www.nongnu.org/avr-libc/u... for details, assuming you're using avr-gcc (WinAVR).
// #define 	SIGNAL(vector)
// SIGNAL(TIMER0_COMPA_vect)
ISR(TIMER0_COMPA_vect)
{ 
  action();
}

ISR(TIMER0_COMPB_vect)
{
  action();
}

static void SetTimerActions(timerAction_t timerAction)
{
    theActions = timerAction;
}

void initTimer(timerAction_t timerAction)
{
    SetTimerActions(timerAction);
    // Timer0 is already used for millis() - we'll just interrupt somewhere in the middle and call the "Compare A" function below
    // This needs more information - common the web as an example but not explained more
    // Its AVR not Arduino
    // • Bit 1 – OCIE0A: Timer/Counter0 Output Compare Match A Interrupt Enable
    // When the OCIE0A bit is written to one, and the I-bit in the status register is set,
    /// the Timer/Counter0 compare match A interrupt is enabled.
    //The corresponding interrupt is executed if a compare OCF0A bit is set in the Timer/Counter 0 interrupt flag register – TIFR0.
    OCR0A = 0x1F;          // set a value into the A register
    TIMSK0 |= _BV(OCIE0A); // this is the Timer counter interupt mask which we set up to bit 1 (I-bit) to be read write
    OCR0B = 0x9E;          // set a value into the B register
    TIMSK0 |= _BV(OCIE0B); // this is the Timer counter interupt mask which we set up to bit 1 (I-bit) to be read write
}