#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "Eigen3/Eigen/Core"
using namespace Eigen;

class sphere : public hittable {
public:
    Vector3d center; // 球心坐标
    double radius;   // 半径
    shared_ptr<material> mat_ptr; // 材质

public:
    sphere() {}
    sphere(Vector3d cen, double r, shared_ptr<material> m)
        : center(cen), radius(r), mat_ptr(m) {};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;

};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    Vector3d oc = r.origin() - center;
    double a = r.direction().dot(r.direction());
    double half_b = oc.dot(r.direction());
    double c = oc.dot(oc) - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant > 0) {
        double root = sqrt(discriminant);
        double temp = (-half_b - root) / a;
        if (t_min < temp && temp < t_max) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            Vector3d outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-half_b + root) / a;
        if (t_min < temp && temp < t_max) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            Vector3d outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}


#endif