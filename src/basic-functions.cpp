#include "basic-functions.h"

#include <iostream>

#include "chassis.h"
#include "position.h"
#include "robot-config.h"
#include "vex.h"
using namespace vex;

void delay(int msec) {  // 等待，单位毫秒
    this_thread::sleep_for(msec);
}

void moveIntaker(double percent) { motor_intaker.spin(directionType::fwd, 127 * percent, voltageUnits::mV); }

void moveIntakerWithRPM(double RPM) { motor_intaker.spin(directionType::fwd, RPM, velocityUnits::rpm); }

void IMUInit(int stable_checking_time, double error_tol) {
    Controller.Screen.print("IMU Calibrating...");
    bool is_stable = false;
    double heading_start = 0;
    double heading_end = 0;
    // 检查是否稳定
    while (!is_stable) {
        Inertial.calibrate();
        waitUntil(!Inertial.isCalibrating());
        heading_start = IMUHeading();
        this_thread::sleep_for(stable_checking_time);
        heading_end = IMUHeading();
        is_stable = degDiffInRange(heading_start, heading_end, error_tol);
    }
    // 输出
    clearControllerScr();
    Controller.Screen.setCursor(5, 1);
    Controller.Screen.print("Done");
    this_thread::sleep_for(1000);
    clearControllerScr();
}

double IMUHeading() {
    double heading = IMURotation();
    while (heading < 0) {
        heading += 360;
    }
    while (heading >= 360) {
        heading -= 360;
    }
    return heading;
}

double IMURotation() {
    double heading = Inertial.rotation(rotationUnits::deg);
    heading = heading / kIMUModCoefficient * 3600;
    return heading;
}

void resetHeading() { Inertial.resetRotation(); }

void setIMUHeading(double heading) { Inertial.setRotation(heading, rotationUnits::deg); }

void clearBrainScr() { Brain.Screen.clearScreen(); }

void clearControllerScr() {
    Controller.Screen.setCursor(5, 1);
    Controller.Screen.print("                                                         ");
}

double getForwardVel() {
    double l1 = motor_base_l1.velocity(rpm);
    double l2 = motor_base_l2.velocity(rpm);
    double l3 = motor_base_l3.velocity(rpm);
    double l4 = motor_base_l4.velocity(rpm);
    double r1 = motor_base_r1.velocity(rpm);
    double r2 = motor_base_r2.velocity(rpm);
    double r3 = motor_base_r3.velocity(rpm);
    double r4 = motor_base_r4.velocity(rpm);
    return (l1 + l2 + l3 + l4 + r1 + r2 + r3 + r4) / 8;
}

double getRotationVel() {
    double l1 = motor_base_l1.velocity(rpm);
    double l2 = motor_base_l2.velocity(rpm);
    double l3 = motor_base_l3.velocity(rpm);
    double l4 = motor_base_l4.velocity(rpm);
    double r1 = motor_base_r1.velocity(rpm);
    double r2 = motor_base_r2.velocity(rpm);
    double r3 = motor_base_r3.velocity(rpm);
    double r4 = motor_base_r4.velocity(rpm);
    return (l1 + l2 + l3 + l4 - r1 - r2 - r3 - r4) / 4;
}

bool checkHeadingInRange(double target, int tol) {
    double diff = IMUHeading() - target;
    while (diff > 180) {
        diff -= 360;
    }
    while (diff <= -180) {
        diff += 360;
    }
    return (std::abs(diff) <= tol);
}

void positionDisplay() {
    Point pos = Position::getInstance()->getPos();
    Controller.Screen.setCursor(5, 3);
    Controller.Screen.print("%.2f %.2f %.2f                 ", pos.x, pos.y, IMUHeading());
}

void modeDisplay(char mode) {
    Controller.Screen.setCursor(5, 1);
    Controller.Screen.print("%c", mode);
}