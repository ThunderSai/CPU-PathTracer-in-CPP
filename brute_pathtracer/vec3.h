#pragma once
#include <bits/stdc++.h>
using namespace std;

static inline double rand01() { 
    return rand() / (RAND_MAX + 1.0); 
}

static inline double clampd(double x, double a, double b) { 
    return x < a ? a : (x > b ? b : x); 
}

struct Vec3 {
    double x, y, z;
    Vec3(): x(0), y(0), z(0) {}
    Vec3(double x_, double y_, double z_): x(x_), y(y_), z(z_) {}

    Vec3 operator-() const { 
        return Vec3(-x, -y, -z);
    }

    Vec3& operator+=(const Vec3& o){ 
        x+=o.x; y+=o.y; z+=o.z; return *this; 
    }

    Vec3& operator*=(double t){ 
        x*=t; y*=t; z*=t; return *this; 
    }

    double len2() const { 
        return x*x + y*y + z*z; 
    }

    double len() const { 
        return sqrt(len2()); 
    }

    Vec3 normalized() const { 
        double L=len(); 
        if(L==0) return Vec3(0,0,0); 
        return Vec3(x/L, y/L, z/L); 
    }

    static Vec3 random(double a=-1.0, double b=1.0) {
        return Vec3(a + (b-a)*rand01(), a + (b-a)*rand01(), a + (b-a)*rand01());
    }
};

inline Vec3 operator+(const Vec3 &a,const Vec3 &b){
    return Vec3(a.x+b.x,a.y+b.y,a.z+b.z);
}

inline Vec3 operator-(const Vec3 &a,const Vec3 &b){
    return Vec3(a.x-b.x,a.y-b.y,a.z-b.z);
}

inline Vec3 operator*(const Vec3 &a,const Vec3 &b){
    return Vec3(a.x*b.x,a.y*b.y,a.z*b.z);
}

inline double dot(const Vec3 &a,const Vec3 &b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline Vec3 cross(const Vec3 &a,const Vec3 &b){ 
    return Vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x); 
}

static Vec3 reflect(const Vec3 &v,const Vec3 &n){ 
    return v - 2*dot(v,n)*n; 
}

static bool refract(const Vec3 &uv,const Vec3 &n,double etai_over_etat, Vec3 &refracted){
    double cosi = -max(-1.0, min(1.0, dot(uv,n)));
    double sin2t = etai_over_etat*etai_over_etat * (1 - cosi*cosi);
    if (sin2t > 1.0) return false;
    double cost = sqrt(1.0 - sin2t);
    refracted = etai_over_etat * (uv + cosi*n) - cost*n;
    return true;
}

static double schlick(double cosine, double ref_idx){
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
}

static Vec3 random_in_unit_sphere(){
    while(true){
        Vec3 p = Vec3::random(-1,1);
        if (p.len2() < 1.0) return p;
    }
}

static Vec3 random_unit_vec(){
    double a = 2.0*M_PI*rand01();
    double z = 2*rand01() - 1;
    double r = sqrt(max(0.0,1.0 - z*z));
    return Vec3(r*cos(a), r*sin(a), z);
}
