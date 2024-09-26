#include "geometry.h"

#include <cmath>
Vector ::Vector(const Point &p1, const Point &p2) : x(p2.x - p1.x), y(p2.y - p1.y) {}

Vector &Vector ::operator=(Vector const &v) {
    if (this == &v)
        return *this;
    else {
        this->x = v.x;
        this->y = v.y;
        return *this;
    }
}

Vector Vector ::rotateTrans(double theta) const {
    double x = cos(deg2rad(dir() + theta)) * mod();
    double y = sin(deg2rad(dir() + theta)) * mod();
    return Vector(x, y);
}