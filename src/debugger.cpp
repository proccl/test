#include "debugger.h"

#include <iostream>

#include "auto-functions.h"
#include "basic-functions.h"
#include "controller.h"
#include "geometry.h"
#include "parameters.h"
#include "robot-config.h"
#include "usercontrol.h"
using namespace std;

void debugControl() {
    modeDisplay('D');
    while (true) {
        baseControl();
        
        if (press_A) {
            press_A = false;
            turnTo(90);
            Chassis::getInstance()->autoSetRobotVel(0, 0);
        }

        if (press_Y) {
            press_Y = false;
            softStartTimerRotate(0, 100, 1000, false);
            Chassis::getInstance()->autoSetRobotVel(0, 0);
        }

        if (press_X) {
            press_X = false;
            Chassis::getInstance()->autoSetRobotVel(0, 0);
        }

        if (press_B) {
            // 清除状态
            Position::getInstance()->setGlobalPosition(0, 0);
            resetHeading();
            press_B = false;
        }
    }
    positionDisplay();
    this_thread::sleep_for(kRefreshTime);
}