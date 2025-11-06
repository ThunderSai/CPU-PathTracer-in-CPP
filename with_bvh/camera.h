#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

struct Camera {
    Vec3 origin, lower_left, horizontal, vertical;
    Camera(const Vec3 &lookfrom, const Vec3 &lookat, const Vec3 &view, double vfov_deg, double aspect);
    Ray get_ray(double s, double t) const;
};

#endif
