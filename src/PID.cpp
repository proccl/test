#include "PID.h"

#include <math.h>

#include <iostream>

#include "calc.h"
#include "geometry.h"
#include "my-timer.h"
#include "vex.h"
using namespace std;

PID::PID() : first_time(true), arrived(false), I_max(20), I_range(50), jump_time(50), D_tol(10) { my_timer.reset(); }

void PID::setFirstTime() { first_time = true; }

void PID::setCoefficient(double _kp, double _ki, double _kd) {
    kp = _kp;
    ki = _ki;
    kd = _kd;
}
void PID::setTarget(double _target) { target = _target; }
void PID::setIMax(double _IMax) { I_max = _IMax; }
void PID::setIRange(double _IRange) { I_range = _IRange; }
void PID::setErrorTolerance(double _errorTol) { error_tol = _errorTol; }
void PID::setDTolerance(double _DTol) { D_tol = _DTol; }
void PID::setJumpTime(double _jumpTime) { jump_time = _jumpTime; }
void PID::setArrived(bool _arrived) { arrived = _arrived; }
bool PID::targetArrived() { return arrived; }
double PID::getOutput() { return output; }

void PID::update(double input) {
    error_curt = target - input;  // calculate current error
    P = kp * error_curt;
    if (first_time) {  // first time to update
        first_time = false;
        error_prev = error_curt;
        error_int = 0;
    }
    error_dev = error_curt - error_prev;  // calculate the derivative of error
    error_prev = error_curt;              // record error
    D = kd * error_dev;                   // calculate D
    if (abs(P) >= I_range) {              // I = 0 for P > I_range
        error_int = 0;
    } else {  // P <= I_range -> Integrate
        error_int += error_curt;
        if (abs(error_int) * ki > I_max)  // Limit I to I_max
            error_int = sign(error_int) * I_max / ki;
    }
    if (sign(error_int) != sign(error_curt) || (abs(error_curt) <= error_tol))  // Clear I for small enough error
        error_int = 0;
    I = ki * error_int;                                     // Calculate I
    if (abs(error_curt) <= error_tol && abs(D) <= D_tol) {  // Exit when staying in tolerated region and
                                                            // maintaining a low enough speed for enough time
        if (my_timer.getTime() >= jump_time) arrived = true;
    } else {
        my_timer.reset();
        arrived = false;
    }
    output = P + I + D;
}

void PosPID::setTarget(Point p) { target_point = p; }

void PosPID::update(Point input) {
    Vector err = target_point - input;
    error_curt = err.mod();  // calculate current error
    P = kp * error_curt;
    if (first_time) {  // first time to update
        first_time = false;
        error_prev = error_curt;
        error_int = 0;
    }
    error_int = error_curt - error_prev;  // calculate the derivative of error
    error_prev = error_curt;              // record error
    D = kd * error_int;                   // calculate D
    if (abs(P) >= I_range) {              // I = 0 for P > I_range
        error_int = 0;
    } else {  // P <= I_range -> Integrate
        error_int += error_curt;
        if (abs(error_int) * ki > I_max)  // Limit I to I_max
            error_int = sign(error_int) * I_max / ki;
    }
    if (sign(error_int) != sign(error_curt) || (abs(error_curt) <= error_tol))  // Clear I for small enough error
        error_int = 0;
    I = ki * error_int;                                     // Calculate I
    if (abs(error_curt) <= error_tol && abs(D) <= D_tol) {  // Exit when staying in tolerated region and
                                                            // maintaining a low enough speed for enough time
        if (my_timer.getTime() >= jump_time) arrived = true;
    } else {
        my_timer.reset();
        arrived = false;
    }
    output = P + I + D;
}

void DirPID::update(double input) {
    error_curt = degNormalize(target - input);  // calculate current error
    P = kp * error_curt;
    if (first_time) {  // first time to update
        first_time = false;
        error_prev = error_curt;
        error_int = 0;
    }
    error_int = error_curt - error_prev;  // calculate the derivative of error
    error_prev = error_curt;              // record error
    D = kd * error_int;                   // calculate D
    if (abs(P) >= I_range) {              // I = 0 for P > I_range
        error_int = 0;
    } else {  // P <= I_range -> Integrate
        error_int += error_curt;
        if (abs(error_int) * ki > I_max)  // Limit I to I_max
            error_int = sign(error_int) * I_max / ki;
    }
    if (sign(error_int) != sign(error_curt) || (abs(error_curt) <= error_tol))  // Clear I for small enough error
        error_int = 0;
    I = ki * error_int;  // Calculate I
    // Exit when staying in tolerated region and maintaining a low enough speed for enough time
    if (abs(error_curt) <= error_tol && abs(D) <= D_tol) {
        if (my_timer.getTime() >= jump_time) arrived = true;
    } else {
        my_timer.reset();
        arrived = false;
    }
    output = P + I + D;
}