#ifndef GEOMETRY_H_
#define GEOMETRY_H_
#include <cmath>

#include "calc.h"

class Point;

class Vector {
public:
    double x;
    double y;
    Vector() : x(0), y(0) {}
    Vector(double x, double y) : x(x), y(y) {}
    Vector(const Point &, const Point &);
    Vector(const Vector &v) : x(v.x), y(v.y) {}
    Vector &operator=(Vector const &);
    Vector operator+(Vector const &v) const { return Vector(x + v.x, y + v.y); }
    Vector operator-(Vector const &v) const { return Vector(x - v.x, y - v.y); }
    double operator*(Vector const &v) const { return x * v.x + y * v.y; }
    Vector operator*(double a) const { return Vector(x * a, y * a); }
    Vector operator/(double a) const { return Vector(x / a, y / a); }
    friend Vector operator*(double a, Vector v) { return Vector(v.x * a, v.y * a); }

    /**
     * @brief Returns module of vector.
     * @return double
     */
    double mod() const { return sqrt(x * x + y * y); }

    /**
     * @brief Returns direction of vector in degree.
     * @return double
     */
    double dir() const { return rad2deg(atan2(y, x)); }

    Vector rotateTrans(double) const;  //

    void setV(double x, double y) {
        x = x;
        y = y;
    }

    void resetV() {
        x = 0;
        y = 0;
    }
};

class Point {
public:
    double x;
    double y;
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
    Point(const Point &p) : x(p.x), y(p.y) {}

    Point &operator=(Point const &p) {
        if (this == &p)
            return *this;
        else {
            this->x = p.x;
            this->y = p.y;
            return *this;
        }
    }

    Point operator+(const Vector &v) const { return Point(x + v.x, y + v.y); }

    Point operator-(const Vector &v) const { return Point(x - v.x, y - v.y); }

    Vector operator-(const Point &p) const { return Vector(x - p.x, y - p.y); }

    friend Point operator+(const Vector &v, const Point &p) { return Point(p.x + v.x, p.y + v.y); }

    void setP(double x, double y) {
        x = x;
        y = y;
    }

    void resetP() {
        x = 0;
        y = 0;
    }
};

#endif
