#include "../include/vec3.h"
#include "../include/utils.h"
#include <cmath>

Vec3 Vec3::random(double a, double b) {
    return Vec3(a + (b-a)*rand01(), a + (b-a)*rand01(), a + (b-a)*rand01());
}

Vec3 reflect(const Vec3 &v,const Vec3 &n){ 
    return v - 2*dot(v,n)*n; 
}

bool refract(const Vec3 &uv,const Vec3 &n,double etai_over_etat, Vec3 &refracted){
    double cosi = -std::max(-1.0, std::min(1.0, dot(uv,n)));
    double sin2t = etai_over_etat*etai_over_etat * (1 - cosi*cosi);

    if (sin2t > 1.0) return false;

    double cost = sqrt(1.0 - sin2t);
    refracted = etai_over_etat * (uv + cosi*n) - cost*n;
    return true;
}

double schlick(double cosine, double ref_idx){
    double r0 = (1 - ref_idx) / (1 + ref_idx);

    r0 = r0 * r0;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
}

Vec3 random_in_unit_sphere(){
    while(true){
        Vec3 p = Vec3::random(-1,1);
        if (p.len2() < 1.0) return p;
    }

}


Vec3 random_unit_vec(){
    double a = 2.0*M_PI*rand01();

    double z = 2*rand01() - 1;
    double r = sqrt(std::max(0.0,1.0 - z*z));
    
    return Vec3(r*cos(a), r*sin(a), z);
}
