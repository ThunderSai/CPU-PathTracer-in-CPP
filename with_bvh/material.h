#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

struct Ray;
struct HitRecord;

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
    bool scatter(const Ray&, const HitRecord &rec, Vec3 &attenuate, Ray &scattered) const override;
};

struct Metal: public Material {
    Vec3 albedo; double fuzz;
    Metal(const Vec3 &a, double f): albedo(a), fuzz(f < 1.0 ? f : 1.0) {}
    bool scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuate, Ray &scattered) const override;
};

struct Dielectric: public Material {
    double ir;
    Dielectric(double index): ir(index) {}
    bool scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuate, Ray &scattered) const override;
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

#endif
