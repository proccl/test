#include "auto-functions.h"

#include <iostream>

#include "PID.h"
#include "basic-functions.h"
#include "chassis.h"
#include "my-timer.h"
#include "position.h"
using namespace std;

/// @brief 瞄准目标点位
/// @param x x轴坐标
/// @param y y轴坐标
/// @param offset 偏移量，默认为0
void aimAt(double tar_x, double tar_y, double offset) {
    Point tar_pos = Point(tar_x, tar_y);
    Point pos = Position::getInstance()->getPos();
    double tar_angle = 90 - (tar_pos - pos).dir() + offset;
    turnTo(tar_angle);
}

/// @brief 车头转向目标角度
/// @param tar_angle 目标角度
/// @param max_speed 最大速度，默认为100, 应为正值
/// @note 结束停车
void turnTo(double tar_angle, double max_speed) {
    DirPID pid;
    pid.setCoefficient(1, 0, 0);
    pid.setJumpTime(200);
    pid.setErrorTolerance(1);
    pid.setIMax(10);
    pid.setIRange(20);
    pid.setTarget(tar_angle);
    pid.setDTolerance(5);
    int time_limit = 1500;
    MyTimer my_timer;
    while (!pid.targetArrived() && my_timer.getTime() <= time_limit) {
        pid.update(IMUHeading());
        double output = pid.getOutput();
        if (abs(output) > max_speed) {
            output = sign(output) * max_speed;
        }
        Chassis::getInstance()->autoSetRobotVel(0, output);
        delay(kRefreshTime);
    }
    Chassis::getInstance()->autoSetRobotVel(0, 0);
}

/// @brief 时间控制前进
/// @param power  电压 -100 ~ 100 负值后退
/// @param duration 时间 ms
/// @note 结束不停车
void timerForward(double power, int duration) {
    Chassis::getInstance()->autoSetRobotVel(power, 0);
    this_thread::sleep_for(duration);
}

/// @brief 控制车辆前进，同时控制车头朝向
/// @param power 电压 -100 ~ 100 负值后退
/// @param duration 时间 ms
/// @param target_heading 车头目标朝向
/// @note 结束不停车
void timerForwardWithHeading(double power, int duration, double target_heading) {
    double kp = 1.0;      // kp = 1.0
    double P_limit = 20;  // P_limit = 20
    auto my_timer = MyTimer();
    while (my_timer.getTime() < duration) {
        double heading_error = degNormalize(target_heading - IMUHeading());
        double power_turn = heading_error * kp;
        if (abs(power_turn) > P_limit) {
            power_turn = sign(power_turn) * P_limit;
        }
        Chassis::getInstance()->autoSetRobotVel(power, power_turn);
        this_thread::sleep_for(kControlCycle);
    }
}

/// @brief 瞄着坐标点向前移动，瞄点与车身朝向夹角不宜过大，时间控制
/// @param power 电压 -100 ~ 100 负值后退
/// @param duration 时间 ms
/// @param tar_x 目标点x坐标
/// @param tar_y 目标带y坐标
/// @param offset 偏移量，默认为0
/// @note 结束不停车
void timerForwardWhileAiming(double power, int duration, double tar_x, double tar_y, double offset) {
    double kp = 1.0;      // kp = 1.0
    double P_limit = 20;  // P_limit = 20
    auto my_timer = MyTimer();
    while (my_timer.getTime() < duration) {
        Point pos = Position::getInstance()->getPos();
        double tar_angle = 90 - (Point(tar_x, tar_y) - pos).dir() + offset;
        double heading_error = degNormalize(tar_angle - IMUHeading());
        double power_turn = heading_error * kp;
        if (abs(power_turn) > P_limit) {
            power_turn = sign(power_turn) * P_limit;
        }
        Chassis::getInstance()->autoSetRobotVel(power, power_turn);
    }
}

/// @brief 以指定电压前进指定距离，无PID控制
/// @param power 电压 -100 ~ 100 负值后退
/// @param target 距离绝对值
/// @note 结束不停车
void distanceForwardRel(double power, double target) {
    Point start_pos = Position::getInstance()->getPos();
    Chassis::getInstance()->autoSetRobotVel(power, 0);
    while ((start_pos - Position::getInstance()->getPos()).mod() < abs(target)) {
        this_thread::sleep_for(kControlCycle);
    }
}

/// @brief 以指定电压前进指定距离，同时保持车头朝向，无PID控制
/// @param power 电压 -100 ~ 100 负值后退
/// @param target 距离绝对值
/// @param target_heading 车头目标朝向
/// @note 结束不停车
void distanceForwardRelWithHeading(double power, double target, double target_heading) {
    Point start_pos = Position::getInstance()->getPos();
    double kp = 1.0;      // kp = 1.0
    double P_limit = 20;  // P_limit = 20
    while ((start_pos - Position::getInstance()->getPos()).mod() < abs(target)) {
        double heading_error = degNormalize(target_heading - IMUHeading());
        double power_turn = heading_error * kp;
        if (abs(power_turn) > P_limit) {
            power_turn = sign(power_turn) * P_limit;
        }
        Chassis::getInstance()->autoSetRobotVel(power, power_turn);
        this_thread::sleep_for(kControlCycle);
    }
}

/// @brief 以指定电压瞄目标点前进指定距离，角度偏移不宜过大
/// @param power 电压 -100 ~ 100
/// @param target 目标距离绝对值
/// @param tar_x 目标x坐标
/// @param tar_y 目标y坐标
/// @note 结束不停车
void distanceForwardRelWhileAiming(double power, double target, double tar_x, double tar_y) {
    Point start_pos = Position::getInstance()->getPos();
    double kp = 1.0;      // kp = 1.0
    double P_limit = 20;  // P_limit = 20
    while ((start_pos - Position::getInstance()->getPos()).mod() < abs(target)) {
        Point pos = Position::getInstance()->getPos();
        double tar_angle = 90 - (Point(tar_x, tar_y) - pos).dir();
        double heading_error = degNormalize(tar_angle - IMUHeading());
        double power_turn = heading_error * kp;
        if (abs(power_turn) > P_limit) {
            power_turn = sign(power_turn) * P_limit;
        }
        Chassis::getInstance()->autoSetRobotVel(power, power_turn);
        this_thread::sleep_for(kControlCycle);
    }
}

/// @brief PID控制前进
/// @param target 目标距离, 负值后退
/// @note 结束停车
void PIDDistanceForwardRel(double target) {
    auto pid = PID();
    int power_sign = sign(target);
    target = abs(target);
    pid.setCoefficient(1, 0, 0);
    pid.setTarget(target);
    pid.setIMax(25);
    pid.setIRange(30);
    pid.setErrorTolerance(10);
    pid.setDTolerance(10);
    pid.setJumpTime(100);
    Point start_pos = Position::getInstance()->getPos();
    double time_limit = 1500;  // 跳出时间限制
    auto my_timer = MyTimer();
    while (!pid.targetArrived() && my_timer.getTime() < time_limit) {
        pid.update((Position::getInstance()->getPos() - start_pos).mod());
        Chassis::getInstance()->autoSetRobotVel(power_sign * pid.getOutput(), 0);
        this_thread::sleep_for(kRefreshTime);
    }
}

/// @brief PID控制前进
/// @param target 目标距离
/// @param target_heading 车头目标朝向
/// @note 结束停车
void PIDDistanceForwardRelWithHeading(double target, double target_heading) {
    double kp = 1.0;      // kp = 1.0
    double P_limit = 20;  // P_limit = 20
    auto pid = PID();
    int power_sign = sign(target);
    target = abs(target);
    pid.setCoefficient(1, 0, 0);
    pid.setTarget(target);
    pid.setIMax(25);
    pid.setIRange(30);
    pid.setErrorTolerance(10);
    pid.setDTolerance(10);
    pid.setJumpTime(100);
    Point start_pos = Position::getInstance()->getPos();
    double time_limit = 1000;  // 跳出时间限制
    auto my_timer = MyTimer();
    while (!pid.targetArrived() && my_timer.getTime() < time_limit) {
        double heading_error = degNormalize(target_heading - IMUHeading());
        double power_turn = heading_error * kp;
        if (abs(power_turn) > P_limit) {
            power_turn = sign(power_turn) * P_limit;
        }
        pid.update((Position::getInstance()->getPos() - start_pos).mod());
        Chassis::getInstance()->autoSetRobotVel(power_sign * pid.getOutput(), power_turn);
        this_thread::sleep_for(kRefreshTime);
    }
    Chassis::getInstance()->autoSetRobotVel(0, 0);
}

/// @brief 软起动前进
/// @param power_init  起始电压 0 ~ 100
/// @param power_final  终止电压 0 ~ 100
/// @param duration  软起动时间 ms
/// @param forward true: 前进，false: 后退; 默认true
/// @note 结束不停车
void softStartTimerForward(float power_init, float power_final, int duration, bool forward_flag) {
    float power_sign = forward_flag ? 1 : -1;
    float step = (power_final - power_init) / duration;
    float power = power_init;
    auto my_timer = MyTimer();
    while (my_timer.getTime() < duration) {
        power = power_init + my_timer.getTime() * step;
        Chassis::getInstance()->autoSetRobotVel(power * power_sign, 0);
    }
}

/// @brief 定时旋转
/// @param power 电压 0 ~ 100
/// @param duration 时间 ms
/// @param clockwise_flag true: 顺时针，flase: 逆时针; 默认true
/// @note 结束不停车
void timerRotate(double power, int duration, bool clockwise_flag) {
    int power_sign = clockwise_flag ? 1 : -1;
    Chassis::getInstance()->autoSetRobotVel(0, power_sign * power);
    this_thread::sleep_for(duration);
}

/// @brief 旋转到相对角度, 无PID控制
/// @param power 电压 0 ~ 100 方向由target正负决定
/// @param target 相对目标角度 -inf ~ inf 正值顺时针
/// @param tol 容差
/// @note 结束不停车
void angleRotateRel(double power, double target, double tol) {
    power = sign(target) * power;
    double target_abs = IMURotation() + target;
    Chassis::getInstance()->autoSetRobotVel(0, power);
    while (abs(IMURotation() - target_abs) > abs(tol)) {
        this_thread::sleep_for(kControlCycle);
    }
}

/// @brief 旋转到绝对角度, 无PID控制
/// @param power 电压 0 ~ 100
/// @param target 绝对目标角度 0 ~ 360
/// @param tol 容差
/// @param clockwise_flag true: 顺时针，false: 逆时针; 默认true
/// @note 结束不停车
void angleRotateAbs(double power, double target, double tol, bool clockwise_flag) {
    int power_sign = clockwise_flag ? 1 : -1;
    double angle_diff = degNormalize(target - IMUHeading());
    Chassis::getInstance()->autoSetRobotVel(0, power_sign * power);
    while (abs(angle_diff) > abs(tol)) {
        angle_diff = degNormalize(target - IMUHeading());
        this_thread::sleep_for(kControlCycle);
    }
}

/// @brief PID控制旋转到相对角度
/// @param target 相对目标角度 -inf ~ inf 正值顺时针
/// @note 结束停车
void PIDAngleRotateRel(double target) {
    double target_abs = target + IMURotation();
    PID pid;
    pid.setCoefficient(1, 0, 0);
    pid.setTarget(target_abs);
    pid.setIMax(25);
    pid.setIRange(30);
    pid.setErrorTolerance(1);
    pid.setDTolerance(5);
    pid.setJumpTime(100);
    double time_limit = 1000 + abs(target * 3);
    auto my_timer = MyTimer();
    while (!pid.targetArrived() && my_timer.getTime() < time_limit) {
        pid.update(IMURotation());
        Chassis::getInstance()->autoSetRobotVel(0, pid.getOutput());
        this_thread::sleep_for(kRefreshTime);
    }
    Chassis::getInstance()->autoSetRobotVel(0, 0);
}

/// @brief PID控制旋转到绝对角度
/// @param target 目标角度 0 ~ 360
/// @note 结束停车
void PIDAngleRotateAbs(double target) {
    DirPID pid;
    pid.setCoefficient(1, 0, 0);
    pid.setTarget(target);
    pid.setIMax(25);
    pid.setIRange(30);
    pid.setErrorTolerance(1);
    pid.setDTolerance(5);
    pid.setJumpTime(100);
    double time_limit = 1000 + abs(degNormalize(target - IMUHeading()) * 3);
    auto my_timer = MyTimer();
    while (!pid.targetArrived() && my_timer.getTime() < time_limit) {
        pid.update(IMUHeading());
        Chassis::getInstance()->autoSetRobotVel(0, pid.getOutput());
        this_thread::sleep_for(kRefreshTime);
    }
    Chassis::getInstance()->autoSetRobotVel(0, 0);
}

/// @brief 软起动旋转
/// @param power_init 起始电压 0~100
/// @param power_final 终止电压 0~100
/// @param duration 软起动时间 ms
/// @param clockwise_flag true: 顺时针，false: 逆时针; 默认true
/// @note 结束不停车
void softStartTimerRotate(double power_init, double power_final, int duration, bool clockwise_flag) {
    double step = (power_final - power_init) / duration;
    double power = power_init;
    int power_sign = clockwise_flag ? 1 : -1;
    auto my_timer = MyTimer();
    while (my_timer.getTime() < duration) {
        power = power + my_timer.getTime() * step;
        Chassis::getInstance()->autoSetRobotVel(0, power_sign * power);
    }
}