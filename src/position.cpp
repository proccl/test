#include "position.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include "basic-functions.h"
#include "geometry.h"
#include "my-timer.h"
#include "parameters.h"
#include "robot-config.h"
using namespace std;

Position::Position() {
    curIMUHeading = 0;
    lastAS5600LValue = curAS5600LValue =
        AS5600_L.value(analogUnits::range12bit),
    lastAS5600RValue = curAS5600RValue =
        AS5600_R.value(analogUnits::range12bit),
    lastLAngle = curLAngle =
        (lastAS5600LValue + curAS5600LValue) / 2 / 4096.0 * 2 * PI;
    lastRAngle = curRAngle =
        (lastAS5600RValue + curAS5600RValue) / 2 / 4096.0 * 2 * PI; // 0 ~ 2PI
    LMileage = RMileage = 0;
    ALLMileage = 0;
    ALRMileage = 0;

    // curLMileage = curRMileage = 0;
    // lastLMileage = lastRMileage = 0;
    curLSpeed = curRSpeed = 0;
    lastLSpeed = lastRSpeed = 0;
    LSpeed[0] = LSpeed[1] = LSpeed[2] = 0;
    RSpeed[0] = RSpeed[1] = RSpeed[2] = 0;
    FiltLSpeed[0] = FiltLSpeed[1] = FiltLSpeed[2] = 0;
    FiltRSpeed[0] = FiltRSpeed[1] = FiltRSpeed[2] = 0;
    selfYSpeed = selfXSpeed = 0;
    globalYSpeed = globalXSpeed = 0;
    globalY = globalX = 0;
    lastTime = 0;
    sampleTime = 0;

    firstFlag = true;

    // flamingo
    fliter_b[0] = 0.0675;
    fliter_b[1] = 0.1349;
    fliter_b[2] = 0.0675;

    fliter_a[0] = 1;
    fliter_a[1] = -1.143;
    fliter_a[2] = 0.4128;

    // legion
    //  fliter_b[0] = 0.0675;
    //  fliter_b[1] = 0.1349;
    //  fliter_b[2] = 0.0675;

    // fliter_a[0] = 1;
    // fliter_a[1] = -1.143;
    // fliter_a[2] = 0.4128;

    // apollo
    //  fliter_b[0] = 0.0201;
    //  fliter_b[1] = 0.0402;
    //  fliter_b[2] = 0.0201;

    // fliter_a[0] = 1;
    // fliter_a[1] = -1.5610;
    // fliter_a[2] = 0.6414;
}

/**
 * @brief 返回陀螺仪朝向，正前方为0，顺时针为正，范围 0 ~ 2PI
 *
 * @return double
 */
void Position::updateInertialHeading() {
    curIMUHeading = deg2rad(IMUHeading());
}

void Position::updateValue() {
    lastAS5600LValue = curAS5600LValue;
    lastAS5600RValue = curAS5600RValue;
    curAS5600LValue = AS5600_L.value(analogUnits::range12bit);
    curAS5600RValue = AS5600_R.value(analogUnits::range12bit);
    // cout << curAS5600LValue << " " << curAS5600RValue << endl;
}

void Position::updateAngle() {
    lastLAngle = curLAngle;
    lastRAngle = curRAngle;
    curLAngle = curAS5600LValue / 4096.0 * 2 * PI;
    curRAngle = curAS5600RValue / 4096.0 * 2 * PI;
}

void Position::updateMileage() {
    double LDeltaAngle = curLAngle - lastLAngle;
    double RDeltaAngle = curRAngle - lastRAngle;
    while (LDeltaAngle < -PI) {
        LDeltaAngle += 2 * PI;
    }
    while (LDeltaAngle > PI) {
        LDeltaAngle -= 2 * PI;
    }
    while (RDeltaAngle < -PI) {
        RDeltaAngle += 2 * PI;
    }
    while (RDeltaAngle > PI) {
        RDeltaAngle -= 2 * PI;
    }
    if (abs(LDeltaAngle) < 0.01) {
        LDeltaAngle = 0;
    }
    if (abs(RDeltaAngle) < 0.01) {
        RDeltaAngle = 0;
    }
    LMileage = -LDeltaAngle * TrackingWheelRadius;

    ALLMileage += LMileage;

    RMileage = RDeltaAngle * TrackingWheelRadius;
    ALRMileage += RMileage;

    // Brain.Screen.setCursor(5, 1);
    // Brain.Screen.print("%.3lf %.3lf ", ALLMileage, ALRMileage);
}

/**
 * @brief 左定位轮速度 cm/s
 *
 * @return float
 */
void Position::updateLSpeed() {
    lastLSpeed = curLSpeed;

    double ret = LMileage * 1000 / sampleTime; // 前向差分法
    // cout << ret << endl;
    if (abs(ret) > 1000 || abs(ret) < 0.001)
        ret = 0;

    // 滤波前的速度
    LSpeed[2] = LSpeed[1];
    LSpeed[1] = LSpeed[0];
    LSpeed[0] = ret;

    // 滤波后的速度
    FiltLSpeed[2] = FiltLSpeed[1];
    FiltLSpeed[1] = FiltLSpeed[0];
    FiltLSpeed[0] = (fliter_b[0] * LSpeed[0] + fliter_b[1] * LSpeed[1] +
                     fliter_b[2] * LSpeed[2] - fliter_a[1] * FiltLSpeed[1] -
                     fliter_a[2] * FiltLSpeed[2]) /
                    fliter_a[0];
    // if (firstFlag) {
    //     // cout << fliter_b[0] * LSpeed[0] << " " << fliter_b[1] * LSpeed[1]
    //     <<
    //     // " "
    //     //      << fliter_b[2] * LSpeed[2] << fliter_a[1] * FiltLSpeed[1] <<
    //     //      endl;
    //     cout << fliter_b[0] << " " << LMileage << " " << sampleTime << endl;
    //     firstFlag = false;
    // }
    if (abs(FiltLSpeed[0]) < 0.001)
        FiltLSpeed[0] = 0;

    curLSpeed = FiltLSpeed[0];
}

/**
 * @brief 右定位轮速度 cm/s
 *
 * @return float
 */
void Position::updateRSpeed() {
    lastRSpeed = curRSpeed;
    double ret = RMileage * 1000 / sampleTime; // 前向差分法
    if (abs(ret) > 1000 || abs(ret) < 0.001)
        ret = 0; // 速度过大或过小，认为是噪声

    // 滤波前的速度
    RSpeed[2] = RSpeed[1];
    RSpeed[1] = RSpeed[0];
    RSpeed[0] = ret;

    // 滤波后的速度
    FiltRSpeed[2] = FiltRSpeed[1];
    FiltRSpeed[1] = FiltRSpeed[0];
    FiltRSpeed[0] = (fliter_b[0] * RSpeed[0] + fliter_b[1] * RSpeed[1] +
                     fliter_b[2] * RSpeed[2] - fliter_a[1] * FiltRSpeed[1] -
                     fliter_a[2] * FiltRSpeed[2]) /
                    fliter_a[0];
    if (abs(FiltRSpeed[0]) < 0.001)
        FiltRSpeed[0] = 0;

    curRSpeed = FiltRSpeed[0];
}

/**
 * @brief 自身坐标系y轴速度 cm/s
 *
 * @return double
 */
void Position::updateSelfYSpeed() {
    selfYSpeed =
        curLSpeed * cos(LEncoderAngle) + curRSpeed * cos(REncoderAngle);
    // cout << selfYSpeed;
}
// void Position::updateSelfYSpeed() {
//     double D = -cos(REncoderAngle) * sin(LEncoderAngle) -
//                cos(LEncoderAngle) * sin(REncoderAngle);
//     double Dy =
//         curLSpeed * -cos(REncoderAngle) - curRSpeed * cos(LEncoderAngle);
//     selfYSpeed = Dy / D;
// }

/**
 * @brief 自身坐标系x轴速度 cm/s
 *
 * @return double
 */
void Position::updateSelfXSpeed() {
    selfXSpeed =
        curLSpeed * sin(LEncoderAngle) - curRSpeed * sin(REncoderAngle);
    // cout << " " << selfXSpeed << endl;
}
// void Position::updateSelfXSpeed() {
//     double D = -cos(REncoderAngle) * sin(LEncoderAngle) -
//                cos(LEncoderAngle) * sin(REncoderAngle);
//     double Dx = curRSpeed * sin(LEncoderAngle) - curLSpeed *
//     sin(REncoderAngle); selfXSpeed = Dx / D;
// }

/**
 * @brief 世界坐标系y轴速度 cm/s
 *
 * @return float
 */
void Position::updateGlobalYSpeed() {
    lastglobalYSpeed = globalYSpeed;
    globalYSpeed =
        selfYSpeed * cos(curIMUHeading) - selfXSpeed * sin(curIMUHeading);
    if (abs(globalYSpeed) < 0.01)
        globalYSpeed = 0;
    if (abs(globalYSpeed) > 250)
        globalYSpeed = lastglobalYSpeed;
    // cout << globalYSpeed;
}

/**
 * @brief 世界坐标系x轴速度 cm/s
 *
 * @return float
 */
void Position::updateGlobalXSpeed() {
    lastglobalXSpeed = globalXSpeed;
    globalXSpeed =
        selfYSpeed * sin(curIMUHeading) + selfXSpeed * cos(curIMUHeading);
    if (abs(globalXSpeed) < 0.01)
        globalXSpeed = 0;
    if (abs(globalXSpeed) > 250)
        globalXSpeed = lastglobalXSpeed;
    // cout << " " << globalXSpeed << endl;
}

/**
 * @brief 更新世界坐标系机器y轴位置（积分器）
 *
 */
void Position::updateGlobalY() {
    double d = globalYSpeed * sampleTime / 1000; // 前向差分法
    // double d = (globalYSpeed + lastglobalYSpeed) * sampleTime / 1000 / 2;
    // //双线性变换法
    if (abs(d) < 0.001)
        return;
    else
        globalY = globalY + d;
}

/**
 * @brief 更新世界坐标系机器x轴位置（积分器）
 *
 * @param speed
 */
void Position::updateGlobalX() {
    double d = globalXSpeed * sampleTime / 1000; // 前向差分法
    // double d = (globalXSpeed + lastglobalXSpeed) * sampleTime / 1000 / 2;
    // //双线性变换法
    if (abs(d) < 0.001)
        return;
    else
        globalX = globalX + d;
}

/**
 * @brief 更新世界坐标系下机器位置（每tick调用）
 *
 */
void Position::updatePos() {
    sampleTime = (Timer.getTimeDouble() - lastTime) * 1000;
    if (sampleTime < 0.001)
        sampleTime = 10;
    lastTime = Timer.getTimeDouble();

    updateInertialHeading();
    // updateLMileage();
    // updateRMileage();
    updateValue();
    updateAngle();
    updateMileage();
    updateLSpeed();
    updateRSpeed();
    updateSelfYSpeed();
    updateSelfXSpeed();
    updateGlobalYSpeed();
    updateGlobalXSpeed();
    updateGlobalY();
    updateGlobalX();
    Point pos = getPos();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("%.3lf %.3lf %.3lf", pos.x, pos.y, IMUHeading());
    Brain.Screen.setCursor(3, 1);
    Brain.Screen.print("%.3lf %.3lf ", selfXSpeed, selfYSpeed);
}

/**
 * @brief 获取当前位置
 *
 * @return Point
 */
Point Position::getPos() const {
    double updateX = globalX - CRdis * (sin(curIMUHeading));
    double updateY = globalY - CRdis * (-1 + cos(curIMUHeading));
    return Point(updateX, updateY);
}

double Position::getXSpeed() const { return globalXSpeed; }

double Position::getYSpeed() const { return globalYSpeed; }

double Position::getLMileage() const { return ALLMileage; }

double Position::getRMileage() const { return ALRMileage; }

void Position::resetXPosition() { globalX = 0; }

void Position::resetYPosition() { globalY = 0; }

void Position::setGlobalPosition(double _x, double _y) {
    globalX = _x;
    globalY = _y;
}

void updatePosition() {
    while (true) {
        Position::getInstance()->updatePos();
        this_thread::sleep_for(positionRefreshTime);
    }
}