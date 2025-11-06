#include "hittable.h"
#include "utils.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <cmath>

bool Sphere::hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const {
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
        if (root < tmin || root > tmax) {
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

bool Sphere::bounding_box(double, double, AABB &box) const {
    box = AABB(center - Vec3(radius, radius, radius), center + Vec3(radius, radius, radius));
    return true;
}

RectPlane::RectPlane(const Vec3 &o, const Vec3 &n, const Vec3 &ud, const Vec3 &vd, 
                     double umin_, double umax_, double vmin_, double vmax_, std::shared_ptr<Material> m)
    : origin(o), normal(n.normalized()), u_dir(ud.normalized()), v_dir(vd.normalized()), 
      umin(umin_), umax(umax_), vmin(vmin_), vmax(vmax_), mat(m) {}

bool RectPlane::hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const {
    double denom = dot(r.d, normal);
    if (fabs(denom) < 1e-8) {
        return false;
    }
    double t = dot(origin - r.o, normal) / denom;
    if (t < tmin || t > tmax){
        return false;
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

bool RectPlane::bounding_box(double, double, AABB &box) const {
    Vec3 corners[4];
    corners[0] = origin + umin*u_dir + vmin*v_dir;
    corners[1] = origin + umin*u_dir + vmax*v_dir;
    corners[2] = origin + umax*u_dir + vmin*v_dir;
    corners[3] = origin + umax*u_dir + vmax*v_dir;
    Vec3 minp = corners[0], maxp = corners[0];
    for (int i=1;i<4;++i){
        minp.x = std::min(minp.x, corners[i].x);
        minp.y = std::min(minp.y, corners[i].y);
        minp.z = std::min(minp.z, corners[i].z);
        maxp.x = std::max(maxp.x, corners[i].x);
        maxp.y = std::max(maxp.y, corners[i].y);
        maxp.z = std::max(maxp.z, corners[i].z);
    }
    Vec3 t = normal * EPS_THICK;
    minp = minp - t;
    maxp = maxp + t;
    box = AABB(minp, maxp);
    return true;
}
