#pragma once
#include "vec3.h"

struct Ray { 
    Vec3 o, d; 
    Ray(){} 
    Ray(const Vec3 &o_, const Vec3 &d_): o(o_), d(d_) {}
    Vec3 at(double t) const { 
        return o + t*d; 
    } 
};
