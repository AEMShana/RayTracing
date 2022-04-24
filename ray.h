#ifndef RAY_H
#define RAY_H

#include "Eigen3/Eigen/Core"

class ray {
private:
    Eigen::Vector3d orig;
    Eigen::Vector3d dir;
public:
    ray() {}
    ray(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction)
        :orig(origin), dir(direction) {}
    Eigen::Vector3d origin() const { return orig; }
    Eigen::Vector3d direction() const { return dir; }
    Eigen::Vector3d at(double t) const { return orig + t * dir; }
};
#endif