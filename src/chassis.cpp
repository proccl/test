#include "chassis.h"

#include <cmath>
#include <iostream>

#include "parameters.h"
#include "robot-config.h"
#include "vex.h"
using namespace vex;
using namespace std;

/// @brief 底盘构造函数
Chassis::Chassis() {
    robot_vel = 0;
    auto_vel = 0;
    manual_vel = 0;
    stop_brake_type = coast;
    robot_vel_rotate = 0;
    manual_vel_rotate = 0;
    auto_vel_rotate = 0;
}

/// @brief 【换算函数】将机器人坐标系下的速度转换为左右侧轮子速度
void Chassis::calcWheelVel() {
    double robot_speed = abs(robot_vel);
    double max_abs_vel = 0;
    double max_abs_wheel_vel = 100;

    wheel_vel_left = robot_vel + robot_vel_rotate;
    wheel_vel_right = robot_vel - robot_vel_rotate;
    wheel_vel_left = wheel_vel_left / max_abs_wheel_vel * 100;
    wheel_vel_right = wheel_vel_right / max_abs_wheel_vel * 100;
}

/// @brief 【换算函数】将轮速转换为对应电机电压
void Chassis::calcWheelVolt() {
    wheel_volt_left = wheel_vel_left * speed_gain;
    if (abs(wheel_volt_left) < 1000) wheel_volt_left = 0;
    wheel_volt_right = wheel_vel_right * speed_gain;
    if (abs(wheel_volt_right) < 1000) wheel_volt_right = 0;
}

/// @brief 按指定电压驱动电机
void Chassis::setMotorVolt() {
    double leff = 1.0;
    double reff = 1.0;
    if (wheel_volt_left == 0) {
        motor_base_l1.stop(stop_brake_type);
        motor_base_l2.stop(stop_brake_type);
        motor_base_l3.stop(stop_brake_type);
        motor_base_l4.stop(stop_brake_type);
    } else {
        motor_base_l1.spin(directionType::fwd, wheel_volt_left * leff, voltageUnits::mV);
        motor_base_l2.spin(directionType::fwd, wheel_volt_left * leff, voltageUnits::mV);
        motor_base_l3.spin(directionType::fwd, wheel_volt_left * leff, voltageUnits::mV);
        motor_base_l4.spin(directionType::fwd, wheel_volt_left * leff, voltageUnits::mV);
    }

    if (wheel_volt_right == 0) {
        motor_base_r1.stop(stop_brake_type);
        motor_base_r2.stop(stop_brake_type);
        motor_base_r3.stop(stop_brake_type);
        motor_base_r4.stop(stop_brake_type);
    } else {
        motor_base_r1.spin(directionType::fwd, wheel_volt_right * reff, voltageUnits::mV);
        motor_base_r2.spin(directionType::fwd, wheel_volt_right * reff, voltageUnits::mV);
        motor_base_r3.spin(directionType::fwd, wheel_volt_right * reff, voltageUnits::mV);
        motor_base_r4.spin(directionType::fwd, wheel_volt_right * reff, voltageUnits::mV);
    }
}

/// @brief 强制底盘刹车（注意会受到电机驱动线程的影响，在使用该函数时确保不与电机驱动线程冲突）
/// @param brake 底盘刹车类型（coast，brake，hold）
void Chassis::chassisBrake(brakeType brake) {
    wheel_volt_left = 0;
    wheel_volt_right = 0;

    motor_base_r1.stop(brakeType::brake);
    motor_base_r2.stop(brakeType::brake);
    motor_base_r3.stop(brakeType::brake);
    motor_base_r4.stop(brakeType::brake);

    motor_base_l1.stop(brakeType::brake);
    motor_base_l2.stop(brakeType::brake);
    motor_base_l3.stop(brakeType::brake);
    motor_base_l4.stop(brakeType::brake);
}

/// @brief 底盘电机驱动函数，将作为单独线程不断刷新
void Chassis::chassisRun() { setMotorVolt(); }

/// @brief 【手动控制】通过机器坐标系速度驱动底盘
/// @param vel 机器坐标系下速度，速度大小在 0-100 之间
/// @param vel_rotate 机器人角速度，范围在-100 - 100
void Chassis::manualSetRobotVel(double vel, double vel_rotate) {
    if (abs(vel) > 100) {
        vel = 100 * sign(vel);
    }
    if (abs(vel_rotate) > 100) {
        vel_rotate = 100 * sign(vel_rotate);
    }
    manual_vel = vel;
    manual_vel_rotate = vel_rotate;
    robot_vel = manual_vel + auto_vel;
    robot_vel_rotate = manual_vel_rotate + auto_vel_rotate;
    if (abs(robot_vel) > 100) {
        robot_vel = 100 * sign(robot_vel);
    }
    if (abs(robot_vel_rotate) > 100) {
        robot_vel_rotate = 100 * sign(robot_vel_rotate);
    }
    calcWheelVel();
    calcWheelVolt();
}

/// @brief 【自动控制】通过机器坐标系速度驱动底盘
/// @param Vel 机器坐标系下速度，速度大小在 0-100 之间
/// @param vel_rotate 机器人角速度，范围在-100 - 100
void Chassis::autoSetRobotVel(double vel, double vel_rotate) {
    if (abs(vel) > 100) {
        vel = 100 * sign(vel);
    }
    if (abs(vel_rotate) > 100) {
        vel_rotate = 100 * sign(vel_rotate);
    }
    auto_vel = vel;
    auto_vel_rotate = vel_rotate;
    robot_vel = manual_vel + auto_vel;
    robot_vel_rotate = manual_vel_rotate + auto_vel_rotate;
    if (abs(robot_vel) > 100) {
        robot_vel = 100 * sign(robot_vel);
    }
    if (abs(robot_vel_rotate) > 100) {
        robot_vel_rotate = 100 * sign(robot_vel_rotate);
    }
    calcWheelVel();
    calcWheelVolt();
}

void Chassis::autoSetWheelVel(double vel_left, double vel_right) {
    robot_vel = (vel_left + vel_right) / 2;
    robot_vel_rotate = (vel_left - vel_right) / 2;
    if (abs(robot_vel) > 100) {
        robot_vel = 100 * sign(robot_vel);
    }
    if (abs(robot_vel_rotate) > 100) {
        robot_vel_rotate = 100 * sign(robot_vel_rotate);
    }
    calcWheelVel();
    calcWheelVolt();
}

/// @brief 设置停止时的刹车类型
/// @param brake
void Chassis::setStopBrakeType(brakeType brake) { stop_brake_type = brake; }

void updateChassis() {
    while (true) {
        Chassis::getInstance()->chassisRun();
        this_thread::sleep_for(kRefreshTime);
    }
}
