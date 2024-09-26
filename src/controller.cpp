#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#include "controller.h"

#include <parameters.h>
#include <stdlib.h>

#include <iostream>

#include "robot-config.h"

// Define the buttons
int t, A1, A2, A3, A4, L1, L2, R1, R2, X, Y, A, B, LEFT, RIGHT, UP, DOWN, last_L1, last_L2, last_R1, last_R2, last_X,
    last_Y, last_A, last_B, last_LEFT, last_RIGHT, last_UP, last_DOWN;

bool press_X = false, press_Y = false, press_A = false, press_B = false, press_UP = false, press_DOWN = false,
     press_LEFT = false, press_RIGHT = false;
bool release_X = false, release_Y = false, release_A = false, release_B = false, release_UP = false,
     release_DOWN = false, release_LEFT = false, release_RIGHT = false;
bool release_L1 = false, press_L1 = false;
bool release_R1 = false, press_R1 = false;
bool release_L2 = false, press_L2 = false;
bool release_R2 = false, press_R2 = false;
bool is_user_control = true;

// 更新手柄按键和摇杆的输入
void defineController() {
    while (true) {
        last_L1 = L1;
        last_L2 = L2;
        last_R1 = R1;
        last_R2 = R2;
        last_X = X;
        last_Y = Y;
        last_A = A;
        last_B = B;
        last_LEFT = LEFT;
        last_RIGHT = RIGHT;
        last_UP = UP;
        last_DOWN = DOWN;
        t = Brain.timer(vex::timeUnits::msec);
        A1 = Controller.Axis1.position(vex::percentUnits::pct);
        A2 = Controller.Axis2.position(vex::percentUnits::pct);
        A3 = Controller.Axis3.position(vex::percentUnits::pct);
        A4 = Controller.Axis4.position(vex::percentUnits::pct);
        L1 = Controller.ButtonL1.pressing();
        L2 = Controller.ButtonL2.pressing();
        R1 = Controller.ButtonR1.pressing();
        R2 = Controller.ButtonR2.pressing();
        X = Controller.ButtonX.pressing();
        Y = Controller.ButtonY.pressing();
        A = Controller.ButtonA.pressing();
        B = Controller.ButtonB.pressing();
        LEFT = Controller.ButtonLeft.pressing();
        RIGHT = Controller.ButtonRight.pressing();
        UP = Controller.ButtonUp.pressing();
        DOWN = Controller.ButtonDown.pressing();

        // pressFlag
        if (X && !last_X) press_X = true;
        if (last_X && !X) release_X = true, press_X = false;
        if (A && !last_A) press_A = true;
        if (last_A && !A) release_A = true, press_A = false;
        if (B && !last_B) press_B = true;
        if (last_B && !B) release_B = true, press_B = false;
        if (Y && !last_Y) press_Y = true;
        if (last_Y && !Y) release_Y = true, press_Y = false;
        if (UP && !last_UP) press_UP = true;
        if (last_UP && !UP) release_UP = true, press_UP = false;
        if (DOWN && !last_DOWN) press_DOWN = true;
        if (last_DOWN && !DOWN) release_DOWN = true, press_DOWN = false;
        if (RIGHT && !last_RIGHT) press_RIGHT = true;
        if (last_RIGHT && !RIGHT) release_RIGHT = true, press_RIGHT = false;
        if (LEFT && !last_LEFT) press_LEFT = true;
        if (last_LEFT && !LEFT) release_LEFT = true, press_LEFT = false;

        if (L1 && !last_L1) press_L1 = true;
        if (last_L1 && !L1) release_L1 = true, press_L1 = false;
        if (R1 && !last_R1) press_R1 = true;
        if (last_R1 && !R1) release_R1 = true, press_R1 = false;
        if (L2 && !last_L2) press_L2 = true;
        if (last_L2 && !L2) release_L2 = true, press_L2 = false;
        if (R2 && !last_R2) press_R2 = true;
        if (last_R2 && !R2) release_R2 = true, press_R2 = false;

        this_thread::sleep_for(kRefreshTime);
    }
}

#endif