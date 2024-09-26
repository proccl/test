#include "PID.h"

#include <math.h>

#include <iostream>

#include "calc.h"
#include "geometry.h"
#include "my-timer.h"
#include "vex.h"
using namespace std;

/**
 * 本文件定义PID控制器
 * 需完成以下内容并在注释中完整解释：
 * PID::update()
 * DirPID::update()
 * PosPID::update()
 * 
 * 样例使用方法：
 * PID pid;
 * pid.setTarget(100);
 * pid.setErrorTolerance(1);
 * pid.setCoefficient(1, 0.1, 0.1);
 * while(!pid.taragetArrived()){
 *      pid.update(curr_value);
 *      output = pid.getOutput();
 *      // do something with output
 * }
 * 
 */

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
    //TODO: calculate the contribution of P, I, D with kp, ki, kd

    
    if (abs(error_curt) <= error_tol) {  // Exit when staying in tolerated region and
                                        // maintaining a low enough speed
        arrived = true;
    }
    output = P + I + D;
}

void PosPID::setTarget(Point p) { target_point = p; }

void PosPID::update(Point input) {
    Vector err = target_point - input;
    error_curt = err.mod();  // calculate current error
    //TODO: calculate the contribution of P, I, D with kp, ki, kd

    
    if (abs(error_curt) <= error_tol) {  // Exit when staying in tolerated region and
                                        // maintaining a low enough speed
        arrived = true;
    }
    output = P + I + D;
}

void DirPID::update(double input) {
    error_curt = degNormalize(target - input);  // calculate current error
    //TODO: calculate the contribution of P, I, D with kp, ki, kd

    
    if (abs(error_curt) <= error_tol) {  // Exit when staying in tolerated region and
                                        // maintaining a low enough speed
        arrived = true;
    }
    output = P + I + D;
}