#ifndef MYTIMER_H_
#define MYTIMER_H_

#include "vex.h"
using namespace vex;

class MyTimer {
  private:
    double start_time;

  public:
    MyTimer();
    MyTimer(double);
    void reset();
    int getTime() const;
    double getTimeDouble() const;
};

#endif