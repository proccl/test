#include "calc.h"

#include <cmath>

/**
 * @brief Returns the sign of a given number.
 * @param x The number to determine the sign of.
 * @return The sign of the number: 1 if positive, -1 if negative, 0 if zero.
 */
int sign(double x) { return (x > 0) - (x < 0); }

/**
 * @brief Converts an angle from radians to degrees.
 * @param rad The angle in radians.
 * @return The angle in degrees.
 */
double rad2deg(double rad) { return rad / M_PI * 180.0; }

/**
 * @brief Converts an angle from degrees to radians.
 * @param deg The angle in degrees.
 * @return The angle in radians.
 */
double deg2rad(double deg) { return deg * M_PI / 180.0; }

/**
 * @brief Normalizes an angle to the range [-180, 180).
 * @param deg The angle in degrees.
 * @return The normalized angle in degrees.
 */
double degNormalize(double deg) {
    while (deg > 180) {
        deg -= 360;
    }
    while (deg <= -180) {
        deg += 360;
    }
    return deg;
}

/**
 * @brief Checks if the difference between two angles is within a given tolerance.
 * @param deg1 The first angle in degrees.
 * @param deg2 The second angle in degrees.
 * @param tol The tolerance value.
 * @return True if the difference between the angles is within the tolerance, false otherwise.
 */
bool degDiffInRange(double deg1, double deg2, double tol) {
    double diff = degNormalize(deg1 - deg2);
    return (std::abs(diff) <= tol);
}