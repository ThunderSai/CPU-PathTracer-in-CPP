#include <bits/stdc++.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "include/vec3.h"
#include "include/ray.h"
#include "include/hittable.h"
#include "include/camera.h"
#include "include/material.h"
#include "include/utils.h"

using namespace std;
using namespace std::chrono;

Vec3 trace(const Ray &r, const Hittable &world, int depth) {
    if (depth <= 0) return Vec3(0,0,0);

    HitRecord rec;
    
    if (world.hit(r, 1e-4, 1e8, rec)) {
        Vec3 emitted = rec.mat->emitted();
        Ray scattered;
        Vec3 atten;
        if (rec.mat->scatter(r, rec, atten, scattered)) {
            Vec3 col = trace(scattered, world, depth-1);
            return emitted + atten * col;
        } else {
            return emitted;
        }
    }

    return Vec3(0,0,0);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto start_time = high_resolution_clock::now();
    cerr << "Starting render (WITH BVH)...\n";

    const int WIDTH = 1920;
    const int HEIGHT = 1080;
    const int SAMPLES = 100;
    const int MAX_DEPTH = 12;

    cout << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

    vector<shared_ptr<Hittable>> objects;

    auto red = make_shared<Lambert>(Vec3(0.65, 0.05, 0.05));
    auto green = make_shared<Lambert>(Vec3(0.12, 0.45, 0.15));
    auto grey = make_shared<Lambert>(Vec3(0.73, 0.73, 0.73));
    auto dark_green = make_shared<Lambert>(Vec3(0.08,0.35,0.12));
    auto chrome = make_shared<Metal>(Vec3(0.98,0.98,0.99), 0.0);
    auto glass = make_shared<Dielectric>(1.5);
    auto window_left_light = make_shared<DiffuseLight>(Vec3(3.0, 3.5, 4.0));
    auto window_right_light = make_shared<DiffuseLight>(Vec3(3.0, 1.0, 0.2));

    double room_len = 2.0;
    double half = room_len / 2.0;

    objects.push_back(make_shared<RectPlane>(Vec3(0,-half,0), Vec3(0,1,0), Vec3(1,0,0), Vec3(0,0,1),
                                     -half, half, -half, half, grey));
    objects.push_back(make_shared<RectPlane>(Vec3(0,half,0), Vec3(0,-1,0), Vec3(1,0,0), Vec3(0,0,1),
                                     -half, half, -half, half, grey));
    objects.push_back(make_shared<RectPlane>(Vec3(0,0,-half), Vec3(0,0,1), Vec3(1,0,0), Vec3(0,1,0),
                                     -half, half, -half, half, grey));
    objects.push_back(make_shared<RectPlane>(Vec3(-half,0,0), Vec3(1,0,0), Vec3(0,0,1), Vec3(0,1,0),
                                     -half, half, -half, half, red));
    objects.push_back(make_shared<RectPlane>(Vec3(half,0,0), Vec3(-1,0,0), Vec3(0,0,1), Vec3(0,1,0),
                                     -half, half, -half, half, green));

    objects.push_back(make_shared<RectPlane>(Vec3(-half+1e-4, 0.3, -0.6), Vec3(1,0,0),
                                     Vec3(0,0,1), Vec3(0,1,0),
                                     -0.2, 0.2, -0.2, 0.2, window_left_light));
    objects.push_back(make_shared<RectPlane>(Vec3(half-1e-4, 0.6, 0.6), Vec3(-1,0,0),
                                     Vec3(0,0,1), Vec3(0,1,0),
                                     -0.18, 0.18, -0.12, 0.12, window_right_light));

    objects.push_back(make_shared<Sphere>(Vec3(0.0, -0.35, 0.0), 0.35, glass));
    objects.push_back(make_shared<Sphere>(Vec3(0.7, -0.45, 0.3), 0.45, chrome));
    objects.push_back(make_shared<Sphere>(Vec3(-0.7, -0.55, -0.4), 0.25, dark_green));

    vector<shared_ptr<Hittable>> objects_copy = objects;
    shared_ptr<Hittable> world = make_shared<BVHNode>(objects_copy, 0, objects_copy.size());

    Vec3 lookfrom(0.0, 0.0, 2.5);
    Vec3 lookat(0.0, -0.2, 0.0);
    Vec3 vup(0,1,0);
    double vfov = 35.0;
    double aspect = double(WIDTH) / double(HEIGHT);
    Camera cam(lookfrom, lookat, vup, vfov, aspect);

    for (int j = HEIGHT-1; j >= 0; --j) {
        cerr << "\rScanlines remaining: " << j << " " << flush;
        for (int i = 0; i < WIDTH; ++i) {
            Vec3 col(0,0,0);
            for (int s = 0; s < SAMPLES; ++s) {
                double u = (i + rand01()) / (WIDTH - 1.0);
                double v = (j + rand01()) / (HEIGHT - 1.0);
                Ray r = cam.get_ray(u, v);
                col += trace(r, *world, MAX_DEPTH);
            }
            col *= (1.0 / SAMPLES);
            int ir = static_cast<int>(256 * clampd(sqrt(col.x), 0.0, 0.999));
            int ig = static_cast<int>(256 * clampd(sqrt(col.y), 0.0, 0.999));
            int ib = static_cast<int>(256 * clampd(sqrt(col.z), 0.0, 0.999));
            cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);

    cerr << "\nRender complete.\n";
    cerr << "Time taken (WITH BVH): " << duration.count() / 1000.0 << " seconds\n";

    return 0;
}
