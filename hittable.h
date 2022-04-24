#ifndef HITTABLE_H
#define HITTABLE_H


#include "Eigen3/Eigen/Core"
#include "ray.h"
using namespace Eigen;

class material;

struct hit_record {
    Vector3d p;
    Vector3d normal;
    shared_ptr<material> mat_ptr;
    double t;
    bool front_face;
    // 规定法线方向永远与射线方向大致相反
    inline void set_face_normal(const ray& r, const Vector3d& outward_normal) {
        front_face = r.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;

};


#endif