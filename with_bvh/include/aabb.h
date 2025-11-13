#ifndef AABB_H
#define AABB_H

#include "ray.h"

struct AABB {
    Vec3 minimum, maximum;
    AABB() {}
    AABB(const Vec3 &a, const Vec3 &b): minimum(a), maximum(b) {}

    Vec3 min() const { 
        return minimum; 
    }

    Vec3 max() const { 
        return maximum; 
    }
    
    bool hit(const Ray &r, double tmin, double tmax) const;
};

AABB surrounding_box(const AABB &box0, const AABB &box1);

#endif
