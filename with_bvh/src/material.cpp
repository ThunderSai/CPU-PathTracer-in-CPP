#include "../include/material.h"
#include "../include/hittable.h"
#include "../include/utils.h"
#include <algorithm>
// lambert scatters ray in ramdom directions
bool Lambert::scatter(const Ray&, const HitRecord &rec, Vec3 &attenuate, Ray &scattered) const {
    Vec3 target = rec.normal + random_unit_vec();
    
    // Agar vector bahut chhota ho gaya toh normal hi use kar lo
    if (target.len2() < 1e-8){ 
    target = rec.normal;
    }
    scattered = Ray(rec.p, target.normalized()); // final bounce ray
    attenuate = albedo; // hrere we giving colour to the surface
    return true;
}
// we treat metal like a mirror perfectly reflrct light but if mrtal if rough we add a lillte randomness named fuzz
bool Metal::scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuate, Ray &scattered) const {
    Vec3 reflected = reflect(in.d.normalized(), rec.normal); // reflection
    // fuzz give little blur effect for better realism.
    scattered = Ray(rec.p, (reflected + fuzz*random_in_unit_sphere()).normalized());
    attenuate = albedo;
     // Reflection sirf tabhi valid jab surface se bahar jaa rahi ho
    return dot(scattered.d, rec.normal) > 0;
}

// we treat dielectric like a glass like object reflection and refraction will follow snails and sclicht law.

bool Dielectric::scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuate, Ray &scattered) const {
    attenuate = Vec3(1,1,1);
    double ref_ratio = rec.front_face ? (1.0/ir) : ir;
    Vec3 unit_d = in.d.normalized();
    // claculating angles to put in formulas.
    double cos_theta = std::min(1.0, std::max(-1.0, -dot(unit_d, rec.normal)));
    double sin_theta = sqrt(std::max(0.0, 1 - cos_theta*cos_theta));
      // Total internal reflection ka check.
    bool cannot_refract = ref_ratio * sin_theta > 1.0;
    Vec3 direction;
     // Reflect ya refract — yaha decision hota hai
    if (cannot_refract || schlick(cos_theta, ref_ratio) > rand01()) {
        direction = reflect(unit_d, rec.normal);
    } else {
        Vec3 refr;
        refract(unit_d, rec.normal, ref_ratio, refr);
        direction = refr;
    }
    scattered = Ray(rec.p, direction.normalized()); // final ray output.
    return true;
}
