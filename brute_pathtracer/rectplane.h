#pragma once
#include "hittable.h"

struct RectPlane: public Hittable {
    Vec3 origin;
    Vec3 normal; 
    Vec3 u_dir, v_dir;
    double umin, umax, vmin, vmax;
    shared_ptr<Material> mat;
    RectPlane(const Vec3 &o,const Vec3 &n,const Vec3 &ud,const Vec3 &vd, double umin_, double umax_, double vmin_, double vmax_, shared_ptr<Material> m)
    : origin(o), normal(n.normalized()), u_dir(ud.normalized()), v_dir(vd.normalized()), umin(umin_), umax(umax_), vmin(vmin_), vmax(vmax_), mat(m) {}
    bool hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const override {
        double denom = dot(r.d, normal);
        if (fabs(denom) < 1e-8) {
            return false;
        }
        double t = dot(origin - r.o, normal) / denom;
        if (t < tmin || t > tmax) {
            return false;
        }
        Vec3 p = r.at(t);
        Vec3 rel = p - origin;
        double u = dot(rel, u_dir);
        double v = dot(rel, v_dir);
        if (u < umin || u > umax || v < vmin || v > vmax){
            return false;
        }
        rec.t = t; rec.p = p; rec.set_face_normal(r, normal);
        rec.mat = mat;
        return true;
    }
};
