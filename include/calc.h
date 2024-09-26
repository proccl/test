#ifndef CALC_H_
#define CALC_H_

int sign(double);

double rad2deg(double);

double deg2rad(double);

double degNormalize(double deg);

bool degDiffInRange(double deg1, double deg2, double tol);

#endif
