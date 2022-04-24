#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = acos(-1.0);


inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180;
}

inline double ffmin(double a, double b) { return a <= b ? a : b; }
inline double ffmax(double a, double b) { return a >= b ? a : b; }


#include "ray.h"
#include "Eigen3/Eigen/Core"
#include "Eigen3/Eigen/Dense"
using namespace Eigen;

#include <functional>
#include <random>

inline double random_double() {
    // 返回在 [0,1) 内的随机数
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<double()> rand_generator =
        std::bind(distribution, generator);
    return rand_generator();
}

inline double random_double(double min, double max) {
    // 返回在 [min,max) 内的随机数
    return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max) {
    // 将 x 限制在 [min,max] 内
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline Vector3d random_vector3d() {
    return Vector3d(random_double(), random_double(), random_double());
}

inline Vector3d random_vector3d(double min, double max) {
    return Vector3d(random_double(min, max), random_double(min, max), random_double(min, max));
}

Vector3d random_in_unit_sphere() {
    // 拒绝法随机生成在单位球内的向量
    Vector3d p = random_vector3d(-1, 1);
    while (p.dot(p) >= 1) p = random_vector3d(-1, 1);
    return p;
}

Vector3d random_in_hemisphere(const Vector3d& normal) {
    // 拒绝法随机生成在单位半球内的向量
    Vector3d in_unit_sphere = random_in_unit_sphere();
    if (normal.dot(in_unit_sphere) > 0.0) return in_unit_sphere;
    return -in_unit_sphere;
}

Vector3d random_unit_vector3d() {
    // 在单位球面上生成一个随机向量
    // 此处使用极坐标
    double a = random_double(0, 2 * pi);
    double z = random_double(-1, 1);
    double r = sqrt(1 - z * z);
    return Vector3d(r * cos(a), r * sin(a), z);
}

// 获得v关于法线n的镜面反射向量
Vector3d reflect(const Vector3d& v, const Vector3d& n) {
    return v - 2 * v.dot(n) * n;
}

#endif