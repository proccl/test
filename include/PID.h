#ifndef PID_H_
#define PID_H_

#include "geometry.h"
#include "my-timer.h"

class PID {
protected:
    double error_curt, error_prev, error_dev, error_int;
    double P, I, D;
    bool first_time;
    bool arrived;
    double kp, ki, kd;
    double target, error_tol, D_tol;
    double I_max,
        I_range;  // I < abs(IMAX); I starts to increase when P < IRangef
    double output;
    double jump_time;
    MyTimer my_timer;

public:
    PID();
    void setFirstTime();
    void setCoefficient(double, double, double);
    virtual void setTarget(double);
    void setIMax(double);
    void setIRange(double);
    void setErrorTolerance(double);
    void setDTolerance(double);
    void setJumpTime(double);
    virtual void update(double input);
    bool targetArrived();
    void setArrived(bool _arrived);
    double getOutput();
};

class PosPID : public PID {
private:
    Point target_point;

public:
    void setTarget(Point p);
    void update(Point input);
};

class DirPID : public PID {
public:
    void update(double input);
};

#endif