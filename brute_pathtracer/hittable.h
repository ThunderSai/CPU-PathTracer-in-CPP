#pragma once
#include "ray.h"
#include <memory>
using namespace std;

struct Material;

struct HitRecord {
    Vec3 p;
    Vec3 normal;
    shared_ptr<Material> mat;
    double t;
    bool front_face;
    void set_face_normal(const Ray &r, const Vec3 &outward){
        front_face = dot(r.d, outward) < 0;
        normal = front_face ? outward : -outward;
    }
};

struct Hittable {
    virtual bool hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const = 0;
    virtual ~Hittable() = default;
};
