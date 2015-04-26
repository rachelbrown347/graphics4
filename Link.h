#pragma once

#include "Vector.h"
#include "Matrix.h"

void drawLink(Vector, Vector);

struct Link {
    double d; //length
    double theta; //azimuth
    double alpha; //elevation
    double a; //offset

    void getVector() {
        Vector origin = {0, 0, 0, 1};
        Vector endpoint = {0, 0, 1, 1};

        Matrix T = {{
            {cos(theta), -sin(theta) * cos(alpha),  sin(theta) * sin(alpha), a * cos(theta)},
            {sin(theta),  cos(theta) * cos(alpha), -cos(theta) * sin(alpha), a * sin(theta)},
            {         0,               sin(alpha),               cos(alpha),              d},
            {         0,                        0,                        0,              1}
        }};
        
        drawLink(T * origin, T * endpoint);

        // printf("newOrigin: %f, %f, %f, %f; newEndpoint: %f, %f, %f, %f;", 
        //     newOrigin.x, newOrigin.y, newOrigin.z, newOrigin.w, 
        //     newEndpoint.x, newEndpoint.y, newEndpoint.z, newEndpoint.w);
    }
};

