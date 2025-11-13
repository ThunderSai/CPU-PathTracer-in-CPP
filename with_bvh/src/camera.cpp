#include "../include/camera.h"
#include <cmath>

Camera::Camera(const Vec3 &lookfrom, const Vec3 &lookat, const Vec3 &view, double vfov_deg, double aspect) {
    double theta = vfov_deg * M_PI / 180.0;
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

Ray Camera::get_ray(double s, double t) const {
    return Ray(origin, (lower_left + s*horizontal + t*vertical - origin).normalized());
}
