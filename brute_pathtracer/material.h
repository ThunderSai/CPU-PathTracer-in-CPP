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

struct Dielectric: public Material {
    double ir;
    Dielectric(double index): ir(index) {}
    bool scatter(const Ray &in, const HitRecord &rec, Vec3 &attanuate, Ray &scattered) const override {
        attanuate = Vec3(1,1,1);
        double ref_ratio = rec.front_face ? (1.0/ir) : ir;
        Vec3 unit_d = in.d.normalized();
        double cos_theta = min(1.0, max(-1.0, -dot(unit_d, rec.normal)));
        double sin_theta = sqrt(max(0.0, 1 - cos_theta*cos_theta));
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
};

struct DiffuseLight: public Material {
    Vec3 emit;
    DiffuseLight(const Vec3 &c): emit(c) {}
    bool scatter(const Ray&, const HitRecord&, Vec3&, Ray&) const override { 
        return false; 
    }
    Vec3 emitted() const override { 
        return emit; 
    }
};
