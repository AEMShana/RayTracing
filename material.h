#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "utils.h"

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec,
        Vector3d& attenuation, ray& scattered) const = 0;


};

// lambertian 漫反射模型
class lambertian :public material {
public:
    Vector3d albedo; // RGB三个通道的反射率

public:
    lambertian(const Vector3d& a) :albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec,
        Vector3d& attenuation, ray& scattered) const {
        // attenuation 衰减率
        Vector3d scatter_direction = rec.normal + random_unit_vector3d();
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

// 金属材质
class metal :public material {
public:
    Vector3d albedo;
    double fuzz; // 模糊程度
public:
    metal(const Vector3d& a, double f) :albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec,
        Vector3d& attenuation, ray& scattered) const {
        Vector3d reflected = reflect(r_in.direction().normalized(), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (scattered.direction().dot(rec.normal) > 0); // dot<0我们认为吸收
    }
};

#endif