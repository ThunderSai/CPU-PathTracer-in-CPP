#include "../include/material.h"
#include "../include/hittable.h"
#include "../include/utils.h"
#include <algorithm>

bool Lambert::scatter(const Ray&, const HitRecord &rec, Vec3 &attenuate, Ray &scattered) const {
    Vec3 target = rec.normal + random_unit_vec();
    if (target.len2() < 1e-8){ 
    target = rec.normal;
    }
    scattered = Ray(rec.p, target.normalized());
    attenuate = albedo;
    return true;
}

bool Metal::scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuate, Ray &scattered) const {
    Vec3 reflected = reflect(in.d.normalized(), rec.normal);
    scattered = Ray(rec.p, (reflected + fuzz*random_in_unit_sphere()).normalized());
    attenuate = albedo;
    return dot(scattered.d, rec.normal) > 0;
}

bool Dielectric::scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuate, Ray &scattered) const {
    attenuate = Vec3(1,1,1);
    double ref_ratio = rec.front_face ? (1.0/ir) : ir;
    Vec3 unit_d = in.d.normalized();
    double cos_theta = std::min(1.0, std::max(-1.0, -dot(unit_d, rec.normal)));
    double sin_theta = sqrt(std::max(0.0, 1 - cos_theta*cos_theta));
    bool cannot_refract = ref_ratio * sin_theta > 1.0;
    Vec3 direction;
    if (cannot_refract || schlick(cos_theta, ref_ratio) > rand01()) {
        direction = reflect(unit_d, rec.normal);
    } else {
        Vec3 refr;
        refract(unit_d, rec.normal, ref_ratio, refr);
        direction = refr;
    }
    scattered = Ray(rec.p, direction.normalized());
    return true;
}
