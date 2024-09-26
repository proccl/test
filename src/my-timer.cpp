#include "my-timer.h"

#include <math.h>

#include "robot-config.h"

MyTimer::MyTimer() { start_time = Brain.Timer.value(); }

MyTimer::MyTimer(double init) { start_time = Brain.Timer.value() + init / 1000; }

void MyTimer::reset() { start_time = Brain.Timer.value(); }

int MyTimer::getTime() const {
    return floor((Brain.Timer.value() - start_time) * 1000);  // return time (msec) from start_time
}

double MyTimer::getTimeDouble() const {
    return Brain.Timer.value() - start_time;  // return time (sec) from start_time
}