#include "raylib.h"
#include "timer.h"

#include <stdlib.h>

void initTimer(Timer* timer, double duration, bool repeat, bool autostart, funcCallback callback, void* data) {
    if (duration < 0) {
        exit(EXIT_FAILURE);
    }
    timer->duration = duration;
    timer->startTime = 0.0;
    timer->active = false;
    timer->repeat = repeat;
    timer->callback = callback;
    timer->data = data;
    if (autostart) {
        activateTimer(timer);
    }
}

void activateTimer(Timer* timer) {
    timer->active = true;
    timer->startTime = GetTime();
}

void deactivateTimer(Timer* timer) {
    timer->active = false;
    timer->startTime = 0.0;
    if (timer->repeat) {
        timer->active = true;
    }
}

void updateTimer(Timer* timer) {
    if (timer->active && GetTime() - timer->startTime >= timer->duration) {
        if (timer->callback != NULL && timer->startTime) {
            timer->callback(timer->data);    
        }
        if (timer->repeat) {
            activateTimer(timer);
        } else {
            deactivateTimer(timer);
        }
    }

}
