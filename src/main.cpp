#include <iostream>

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

int main() {
    // Inertial校准
    IMUInit();

    // 线程管理
    thread Tchassis(updateChassis);
    thread Tposition(updatePosition);
    thread Tcontroller(defineController);

    userControl();

    while (true) {
        positionDisplay();

        // Allow other tasks to run
        this_thread::sleep_for(10);
    }
}
