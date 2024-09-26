#ifndef AUTON_FUNCTIONS_H_
#define AUTON_FUNCTIONS_H_
#include <iostream>

#include "PID.h"
#include "basic-functions.h"
#include "chassis.h"
#include "my-timer.h"
#include "position.h"

// 转向函数
void aimAt(double tar_x, double tar_y, double offset = 0);
void turnTo(double tar_angle, double max_speed = 100);

// 前进/后退函数
void timerForward(double power, int duration);
void timerForwardWithHeading(double power, int duration, double target_heading);
void timerForwardWhileAiming(double power, int duration, double tar_x, double tar_y, double offset = 0);
void softStartTimerForward(double power_init, double power_final, int duration, bool forward_flag = true);
void distanceForwardRel(double power, double target);
void distanceForwardRelWithHeading(double power, double target, double target_heading);
void distanceForwardRelWhileAiming(double power, double tar_pos, double tar_x, double tar_y);
void PIDDistanceForwardRel(double target);
void PIDDistanceForwardRelWithHeading(double target, double target_heading);

// 旋转函数
void timerRotate(double power, int duration, bool clockwise_flag = true);
void softStartTimerRotate(double power_init, double power_final, int duration, bool clockwise_flag = true);
void angleRotateRel(double power, double target, double tol = 1);
void angleRotateAbs(double power, double target, double tol = 1, bool clockwise_flag = true);
void PIDAngleRotateRel(double target);
void PIDAngleRotateAbs(double target);

#endif