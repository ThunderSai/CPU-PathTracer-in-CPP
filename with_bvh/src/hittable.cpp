#include "../include/hittable.h"
#include "../include/utils.h"
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

BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end) {
    std::random_device rd;
    std::default_random_engine rng(rd());
    int axis = int(3 * rand01());
    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start+1])) {
            left = objects[start];
            right = objects[start+1];
        } else {
            left = objects[start+1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);
        size_t mid = start + object_span / 2;
        left = std::make_shared<BVHNode>(objects, start, mid);
        right = std::make_shared<BVHNode>(objects, mid, end);
    }

    AABB box_left, box_right;
    if (!left->bounding_box(0,0, box_left) || !right->bounding_box(0,0, box_right))
        std::cerr << "No bounding box in BVHNode constructor.\n";
    box = surrounding_box(box_left, box_right);
}

bool BVHNode::hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const {
    if (!box.hit(r, tmin, tmax)) return false;
    HitRecord leftRec, rightRec;
    
    bool hit_left = left->hit(r, tmin, tmax, leftRec);
    bool hit_right = right->hit(r, tmin, hit_left ? leftRec.t : tmax, rightRec);

    if (hit_right) {
        rec = rightRec;
        return true;
    }

    if (hit_left) {
        rec = leftRec;
        return true;
    }

    return false;
}

bool BVHNode::bounding_box(double, double, AABB &out_box) const {
    out_box = box;
    return true;
}

bool BVHNode::box_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis) {
    AABB box_a, box_b;
    if (!a->bounding_box(0,0,box_a) || !b->bounding_box(0,0,box_b))
        std::cerr << "No bounding box in box_compare.\n";
    if (axis == 0) return box_a.min().x < box_b.min().x;
    else if (axis == 1) return box_a.min().y < box_b.min().y;
    else return box_a.min().z < box_b.min().z;
}

bool BVHNode::box_x_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) { 
    return box_compare(a,b,0); 
}

bool BVHNode::box_y_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) { 
    return box_compare(a,b,1); 
}

bool BVHNode::box_z_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) { 
    return box_compare(a,b,2); 
}
