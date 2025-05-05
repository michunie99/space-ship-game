#include <stdlib.h>

#ifndef TIMER_H
#define TIMER_H


typedef void (*funcCallback)(void* data);

typedef struct {
    double duration;
    double startTime;
    bool active;
    bool repeat;
    funcCallback callback;
    void *data;

} Timer;


void initTimer(Timer* timer, double duration, bool repeat, bool autostart, funcCallback callback, void* data);
void activateTimer(Timer* timer);
void deactivateTimer(Timer* timer);
void updateTimer(Timer* timer);

#endif