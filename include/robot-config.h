#ifndef ROBOT_CONFIG_H_
#define ROBOT_CONFIG_H_
#include "vex.h"

using namespace vex;

extern brain Brain;

// VEXcode device constructors

// 定义八电机底盘的电机对象（端口号+转动方向）
extern motor motor_base_l1;
extern motor motor_base_l2;
extern motor motor_base_l3;
extern motor motor_base_l4;

extern motor motor_base_r1;
extern motor motor_base_r2;
extern motor motor_base_r3;
extern motor motor_base_r4;

// 定义intaker电机对象（端口号+转动方向）
extern motor motor_intaker;
// 定义Controller对象，用于读取遥控器信息
extern controller Controller;
// 定义陀螺仪对象
extern inertial Inertial;
// 定义左右编码器对象
extern analog_in AS5600_L;
extern analog_in AS5600_R;

// 气缸控制
// extern digital_out name;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);

#endif