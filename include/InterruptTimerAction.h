#ifndef InterruptTimerAction_H
#define InterruptTimerAction_H
#include <Arduino.h>

typedef void (*timerAction_t)(unsigned long);
//InterruptTimerAction(timerAction_t intimer_actiontv);
// void SetTimerActions(timerAction_t timerAction);

void initTimer(timerAction_t timerAction);

#endif
