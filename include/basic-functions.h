#ifndef BASIC_FUNCTIONS_H_
#define BASIC_FUNCTIONS_H_

void delay(int msec);

// Basic movement
void moveIntaker(double percent);
void moveIntakerWithRPM(double RPM);

// IMU functions
void IMUInit(int stable_check_time = 2000, double error_tol = 0.2);
double IMUHeading();
double IMURotation();
void resetHeading();
void setIMUHeading(double heading);
bool checkHeadingInRange(double target, int tol);

// Screen functions
void clearBrainScr();
void clearControllerScr();

// Output functions
double getForwardVel();
double getRotationVel();

// Display functions
void positionDisplay();
void modeDisplay(char mode);

#endif