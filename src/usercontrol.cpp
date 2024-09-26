#include "usercontrol.h"

#include <iostream>

#include "basic-functions.h"
#include "chassis.h"
#include "controller.h"
#include "parameters.h"
#include "position.h"
#include "vex.h"
using namespace std;
using namespace vex;

// 底盘车头控制
void baseControl() {
    static bool is_reversed = false;
    double a1, a3, a4;
    a1 = (abs(A1) < 3) ? 0 : A1;
    a3 = (abs(A3) < 3) ? 0 : A3;
    a4 = (abs(A4) < 30) ? 0 : A4;
    if (press_LEFT) {
        press_LEFT = false;
        is_reversed = !is_reversed;
        if (is_reversed) {
            Controller.rumble("-");
        }
        if (!is_reversed) {
            Controller.rumble(".");
        }
    }
    if (is_reversed) {
        a3 = -a3;
    }
    Chassis::getInstance()->manualSetRobotVel(a3, a1);
}

// 底盘锁定模式切换
void baseStopControl() {
    static bool is_brake_type_hold = false;
    if (press_A) {
        press_A = false;
        is_brake_type_hold = !is_brake_type_hold;
        if (is_brake_type_hold) {
            Chassis::getInstance()->chassisBrake(brakeType::hold);
            Chassis::getInstance()->setStopBrakeType(brakeType::hold);
        } else {
            Chassis::getInstance()->chassisBrake(brakeType::coast);
            Chassis::getInstance()->setStopBrakeType(brakeType::coast);
        }
    }
}

void intakerControl() {
    if (R1) {
        moveIntaker(100);
    } else if (R2) {
        moveIntaker(-100);
    } else {
        moveIntaker(0);
    }
}

void userControl() {
    modeDisplay('U');
    vex::thread *T = nullptr;
    while (true) {
        // // ---------------------- 技能赛手动自动切换 ------------------------
        // // --------- 手动 -> 自动：按RIGHT键 / 自动 -> 手动：按DOWN键
        // --------- if (press_RIGHT && T == nullptr) {  // 进入自动
        //     press_RIGHT = false;
        //     is_user_control = false;
        //     Chassis::getInstance()->manualSetRobotVel(0, 0);
        //     T = new thread(autonomous);
        // }
        // if (press_DOWN && T != nullptr) {  // 中断自动进手动
        //     press_DOWN = false;
        //     is_user_control = true;
        //     T->interrupt();
        //     delete T;
        //     T = nullptr;
        //     Chassis::getInstance()->autoSetRobotVel(0, 0);
        // }
        // // ----------------------------------------------------------------

        if (is_user_control) {
            // 底盘控制
            baseControl();
            // baseStopControl();

            // 其余组件控制
            intakerControl();
        }

        this_thread::sleep_for(kRefreshTime);

#ifdef DEBUG
        positionDisplay();
#endif
    }
}
