#include "../include/camera.h"
#include <cmath>

Camera::Camera(const Vec3 &lookfrom, const Vec3 &lookat, const Vec3 &view, double vfov_deg, double aspect) {
    double theta = vfov_deg * M_PI / 180.0; // dregree to radian convert
    double half_h = tan(theta/2); // half of view point
    double half_w = aspect * half_h;
    Vec3 w = (lookfrom - lookat).normalized();
    Vec3 u = cross(view, w).normalized();
    Vec3 v = cross(w, u);
    origin = lookfrom; // camera posotion we selected
    lower_left = origin - half_w*u - half_h*v - w; // lower left corner of our view point
    horizontal = 2*half_w*u;  // full horizontal span of the viewport
    vertical = 2*half_h*v;  // full vertical span of the viewport
}

Ray Camera::get_ray(double s, double t) const {
      // create a ray moving  through point (s, t) on the viewport.
    return Ray(origin, (lower_left + s*horizontal + t*vertical - origin).normalized());
}
