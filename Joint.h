#pragma once
#include <cmath>
#include <cassert>

#include <Vector.h>

struct Link {
    Vector joint;
};

struct Rotator : Link {
    double length;
    Vector axis;

    Vector evaluate(double theta) {
        // theta is relative to ??
        Vector px = joint.x + length * sin(theta);
        Vector py = joint.y
        return joint + 
    }
};