#pragma once
#include "vec3.h"
#include "ray.h"
#include "hittable.h"

struct Material {
    virtual bool scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuate, Ray &scattered) const = 0;

    virtual Vec3 emitted() const { 
        return Vec3(0,0,0); 
    }

    virtual ~Material() = default;
};

struct Lambert: public Material {
    Vec3 albedo;
    Lambert(const Vec3 &a): albedo(a) {}

    bool scatter(const Ray&, const HitRecord &rec, Vec3 &attenuate, Ray &scattered) const override {
        Vec3 target = rec.normal + random_unit_vec();
        if (target.len2() < 1e-8) {
            target = rec.normal;
        }
        scattered = Ray(rec.p, target.normalized());
        attenuate = albedo;
        return true;
    }
};
