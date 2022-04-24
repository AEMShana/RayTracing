#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"
using namespace Eigen;

class camera {
public:
    Vector3d lower_left_corner;
    Vector3d horizontal;
    Vector3d vertical;
    Vector3d origin;

public:
    camera() {
        lower_left_corner = Vector3d(-3, -2, -2);
        horizontal = Vector3d(6, 0, 0);
        vertical = Vector3d(0, 4, 0);
        origin = Vector3d(0, 0, 0);
    }

    ray get_ray(double u, double v) {
        Vector3d dir = lower_left_corner + u * horizontal + v * vertical;
        return ray(origin, dir);
    }
};

#endif