#include <iostream>

#include "autonomous.h"
#include "basic-functions.h"
#include "chassis.h"
#include "controller.h"
#include "debugger.h"
#include "position.h"
#include "robot-config.h"
#include "usercontrol.h"
#include "vex.h"
using namespace vex;
using namespace std;

// 定义Competiton会奇妙进入手动程序
#ifdef COMPETITION
competition Competition;
#endif

int main() {
    // Inertial校准
    IMUInit();

    // 线程管理
    thread Tchassis(updateChassis);
    thread Tposition(updatePosition);
    thread Tcontroller(defineController);

#ifdef COMPETITION
    Competition.autonomous(autonomous);
    Competition.drivercontrol(userControl);
#endif
#ifdef DEBUG
    thread DebugControl(debugControl);
#endif

    while (true) {
        positionDisplay();

        // Allow other tasks to run
        this_thread::sleep_for(10);
    }
}
