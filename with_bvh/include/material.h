#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

struct Ray;
struct HitRecord;

// Base class sab materials ke liye
// Ye batata hai ray surface pe aake kya karegi
struct Material {
// scatter: ray ka naya direction aur color decide karta hai scatter in random directio
    virtual bool scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuate, Ray &scattered) const = 0;
 // emitted: agar material light nikalta hai to yaha se color milega

    virtual Vec3 emitted() const { 
        return Vec3(0,0,0); 
    }
    virtual ~Material() = default;
};
// Lambert: matte surface (painted wall jaisa)
struct Lambert: public Material {
    Vec3 albedo;
    Lambert(const Vec3 &a): albedo(a) {}
    bool scatter(const Ray&, const HitRecord &rec, Vec3 &attenuate, Ray &scattered) const override;
};
// Metal: shiny surface (mirror jaisa)
struct Metal: public Material {
    Vec3 albedo; double fuzz; // fuss to add randomness for rough metal
    Metal(const Vec3 &a, double f): albedo(a), fuzz(f < 1.0 ? f : 1.0) {}
    bool scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuate, Ray &scattered) const override;
};
// Dielectric: transparent (glass/water) we use snails law and schlet law
struct Dielectric: public Material {
    double ir;
    Dielectric(double index): ir(index) {}
    bool scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuate, Ray &scattered) const override;
};
// DiffuseLight is used if material itself is the light source.
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
