#include "autonomous.h"

#include <iostream>

#include "auto-functions.h"
#include "chassis.h"
#include "vex.h"
using namespace std;
using namespace vex;

void autonomous() {
    modeDisplay('A');

    // 自动路线
    turnTo(90);

    // 路线结束后不要退出自动函数
    while (true) {
        this_thread::sleep_for(10);
    }
}