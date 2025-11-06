#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <algorithm>

struct Vec3 {
    double x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

    Vec3 operator-() const { return Vec3(-x, -y, -z); }

    Vec3& operator+=(const Vec3& o) {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }

    Vec3& operator*=(double t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    double len2() const { return x * x + y * y + z * z; }
    double len() const { return std::sqrt(len2()); }

    Vec3 normalized() const {
        double L = len();
        if (L == 0) return Vec3(0, 0, 0);
        return Vec3(x / L, y / L, z / L);
    }

    static Vec3 random(double a = -1.0, double b = 1.0);
};

inline Vec3 operator+(const Vec3& a, const Vec3& b) {
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vec3 operator-(const Vec3& a, const Vec3& b) {
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vec3 operator*(const Vec3& a, const Vec3& b) {
    return Vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline Vec3 operator*(double t, const Vec3& v) {
    return Vec3(t * v.x, t * v.y, t * v.z);
}

inline Vec3 operator*(const Vec3& v, double t) {
    return t * v;
}

inline Vec3 operator/(const Vec3& v, double t) {
    return Vec3(v.x / t, v.y / t, v.z / t);
}

inline double dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

Vec3 reflect(const Vec3& v, const Vec3& n);
bool refract(const Vec3& uv, const Vec3& n, double etai_over_etat, Vec3& refracted);
double schlick(double cosine, double ref_idx);
Vec3 random_in_unit_sphere();
Vec3 random_unit_vec();

#endif
