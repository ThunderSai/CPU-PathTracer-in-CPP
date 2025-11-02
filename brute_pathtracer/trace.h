#pragma once
#include "scene.h"
#include "material.h"

Vec3 trace(const Ray &r, const Scene &scene, int depth) {
    if (depth <= 0) {
        return Vec3(0,0,0);
        
    HitRecord rec;
    if (scene.hit(r, 1e-4, 1e8, rec)) {
        Vec3 emitted = rec.mat->emitted();
        Ray scattered;
        Vec3 attenuate;
        if (rec.mat->scatter(r, rec, attenuate, scattered)) {
            Vec3 col = trace(scattered, scene, depth-1);
            return emitted + attenuate * col;
        } else {
            return emitted;
        }
    }
    return Vec3(0,0,0);
}
