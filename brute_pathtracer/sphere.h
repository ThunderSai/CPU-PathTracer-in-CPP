#pragma once
#include "hittable.h"

struct Sphere: public Hittable {

    Vec3 center;
    double radius;
    shared_ptr<Material> mat;

    Sphere(const Vec3 &c, double r, shared_ptr<Material> m): center(c), radius(r), mat(m) {}

    bool hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const override {

        Vec3 oc = r.o - center;
        double a = dot(r.d, r.d);
        double half_b = dot(oc, r.d);
        double c = dot(oc,oc) - radius*radius;
        double disc = half_b*half_b - a*c;
        if (disc <= 0) return false;
        double sq = sqrt(disc);
        double root = (-half_b - sq)/a;

        if (root < tmin || root > tmax) {
            root = (-half_b + sq)/a;
            if (root < tmin || root > tmax){
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        Vec3 outward = (rec.p - center) / radius;
        rec.set_face_normal(r, outward);
        rec.mat = mat;
        return true;
        
    }
};
