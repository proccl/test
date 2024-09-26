#ifndef PARAMETERS_H_
#define PARAMETERS_H_
#include <math.h>
#include "calc.h"
#include "geometry.h"
#include "vex.h"

const double kPI = M_PI;
const float kWheelRadius = 3.492; // 主动轮半径 cm

const float kControlCycle = 10;
const float kRefreshTime = 10;        // 刷新时间 ms
const float kPositionRefreshTime = 3; // 定位刷新时间 ms

// Inertial sensor correction coefficient, obtained by rotate robot 3600 deg and
// read the origin data got from IMUHeading.
const float kIMUModCoefficient = 3580.23;

const double PI = M_PI;
const float WheelRadius = 3.492;         // 主动轮半径 cm
const float TrackingWheelRadius = 1.905; // 定位轮半径 cm

const double LEncoderAngle = acos(33.547 / 50);
const double REncoderAngle = acos(34.006 / 50);

const double CRdis = 0; // 旋转中心和定位轮交点的距离 cm

const float RefreshTime = 10;         // 刷新时间 ms
const float positionRefreshTime = 10; // 定位刷新时间 ms

// extern mutex m;

const int CENTER_FOV = 150;
const double XOffsetFromCenter = 0;
const double YOffsetFromCenter = 6.75;
const Vector OffsetFromCenter = Vector(XOffsetFromCenter, YOffsetFromCenter);
const double ang_ini = 0;
// centre at 158
// extern bool fourBarFlag;

// #define VisionSensor__SIG_TAR VisionSensor__SIG_RED
// #define VisionSensor__SIG_TAR VisionSensor__SIG_BLUE

#endif