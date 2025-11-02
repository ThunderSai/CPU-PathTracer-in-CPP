#pragma once
#include "vec3.h"
#include "ray.h"

struct Camera {

    Vec3 origin, lower_left, horizontal, vertical;

    Camera(const Vec3 &lookfrom, const Vec3 &lookat, const Vec3 &view, double fov_deg, double aspect){
        double theta = fov_deg * M_PI / 180.0;
        double half_h = tan(theta/2);
        double half_w = aspect * half_h;
        Vec3 w = (lookfrom - lookat).normalized();
        Vec3 u = cross(view, w).normalized();
        Vec3 v = cross(w, u);
        origin = lookfrom;
        lower_left = origin - half_w*u - half_h*v - w;
        horizontal = 2*half_w*u;
        vertical = 2*half_h*v;
    }

    Ray get_ray(double s, double t) const {
        return Ray(origin, (lower_left + s*horizontal + t*vertical - origin).normalized());
    }
};
