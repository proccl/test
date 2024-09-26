#ifndef CHASSIS_H_
#define CHASSIS_H_

#include <math.h>

#include "PID.h"
#include "calc.h"
#include "geometry.h"
#include "vex.h"

class Chassis {
private:
    /*----------------------------------------------------------------------------*/
    /*    为了融合手动与自动控制 */
    /*    将底盘速度分为手动控制部分与自动控制部分 */
    /*    二者相加后为底盘控制速度 */
    /*----------------------------------------------------------------------------*/

    // 机器人坐标系速度，机器朝向为y轴，机器人朝向的右边为x轴
    double robot_vel;

    // 机器人角速度
    double robot_vel_rotate;

    // 机器人坐标系下手动控制的速度
    double manual_vel;
    double manual_vel_rotate;
    // 机器人坐标系下自动控制的速度
    double auto_vel;
    double auto_vel_rotate;

    double wheel_vel_left;
    double wheel_vel_right;
    double wheel_volt_left;
    double wheel_volt_right;

    vex::brakeType stop_brake_type;

    double speed_gain = 127;

    Vector calcRobotVel(Vector Vel);
    void calcWheelVel();
    void calcWheelVolt();
    void setMotorVolt();

    // double updateRobotHeading();

public:
    static Chassis *getInstance() {
        static Chassis *c = NULL;
        if (c == NULL) {
            c = new Chassis();
        }
        return c;
    }
    static void deleteInstance() {
        Chassis *c = Chassis::getInstance();
        if (c != NULL) {
            delete c;
            c = NULL;
        }
    }
    Chassis();
    void setStopBrakeType(brakeType brake);
    void manualSetRobotVel(double vel, double vel_rotate);
    void autoSetRobotVel(double vel, double vel_rotate);
    void autoSetWheelVel(double vel_left, double vel_right);
    void chassisBrake(brakeType brake);
    void chassisRun();
};

void updateChassis();

#endif