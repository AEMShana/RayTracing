#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "Eigen3/Eigen/Core"
#include "Eigen3/Eigen/Dense"
using namespace Eigen;

class triangle :public hittable {
public:
    Vector3d a, b, c;  // 三个顶点的坐标
    shared_ptr<material> mat_ptr;  // 材质
public:
    triangle() {}
    triangle(Vector3d a, Vector3d b, Vector3d c, shared_ptr<material> m)
        :a(a), b(b), c(c), mat_ptr(m) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;

};

bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // Möller Trumbore Algorithm
    Vector3d e1 = b - a, e2 = c - a;
    Vector3d s = r.origin() - a;
    Vector3d s1 = r.direction().cross(e2);
    Vector3d s2 = s.cross(e1);
    double div = s1.dot(e1);
    double t = s2.dot(e2) / div;
    double b1 = s1.dot(s) / div, b2 = s2.dot(r.direction()) / div;
    if (0.0 <= b1 && b1 <= 1.0 && 0.0 <= b2 && b2 <= 1.0 && 0.0 <= 1.0 - b1 - b2 && 1.0 - b1 - b2 <= 1.0) {
        if (t_min < t && t < t_max) {
            rec.t = t;
            rec.p = r.at(rec.t);
            Vector3d outward_normal = e1.cross(e2);
            outward_normal.normalize();
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

#endif