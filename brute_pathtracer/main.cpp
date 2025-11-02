#include <bits/stdc++.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "vec3.h"
#include "ray.h"
#include "camera.h"
#include "material.h"
#include "sphere.h"
#include "rectplane.h"
#include "scene.h"
#include "trace.h"

using namespace std;
using namespace std::chrono;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int WIDTH = 1920;
    const int HEIGHT = 1080;
    const int SAMPLES = 100;
    const int MAX_DEPTH = 12;

    cout << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

    Scene scene;

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

    scene.add(make_shared<RectPlane>(Vec3(0,-half,0), Vec3(0,1,0), Vec3(1,0,0), Vec3(0,0,1), -half, half, -half, half, grey));
    scene.add(make_shared<RectPlane>(Vec3(0,half,0), Vec3(0,-1,0), Vec3(1,0,0), Vec3(0,0,1), -half, half, -half, half, grey));
    scene.add(make_shared<RectPlane>(Vec3(0,0,-half), Vec3(0,0,1), Vec3(1,0,0), Vec3(0,1,0), -half, half, -half, half, grey));
    scene.add(make_shared<RectPlane>(Vec3(-half,0,0), Vec3(1,0,0), Vec3(0,0,1), Vec3(0,1,0), -half, half, -half, half, red));
    scene.add(make_shared<RectPlane>(Vec3(half,0,0), Vec3(-1,0,0), Vec3(0,0,1), Vec3(0,1,0), -half, half, -half, half, green));

    scene.add(make_shared<RectPlane>(Vec3(-half+1e-4, 0.3, -0.6), Vec3(1,0,0), Vec3(0,0,1), Vec3(0,1,0), -0.2, 0.2, -0.2, 0.2, window_left_light));
    scene.add(make_shared<RectPlane>(Vec3(half-1e-4, 0.6, 0.6), Vec3(-1,0,0), Vec3(0,0,1), Vec3(0,1,0), -0.18, 0.18, -0.12, 0.12, window_right_light));

    scene.add(make_shared<Sphere>(Vec3(0.0, -0.35, 0.0), 0.35, glass));
    scene.add(make_shared<Sphere>(Vec3(0.7, -0.45, 0.3), 0.45, chrome));
    scene.add(make_shared<Sphere>(Vec3(-0.7, -0.55, -0.4), 0.25, dark_green));

    Vec3 lookfrom(0.0, 0.0, 2.5);
    Vec3 lookat(0.0, -0.2, 0.0);
    Vec3 viewup(0,1,0);
    double vfov = 35.0;
    double aspect = double(WIDTH) / double(HEIGHT);
    Camera cam(lookfrom, lookat, viewup, vfov, aspect);

    for (int j = HEIGHT-1; j >= 0; --j) {
        cerr << "\rScanlines remaining: " << j << " " << flush;
        for (int i = 0; i < WIDTH; ++i) {
            Vec3 col(0,0,0);
            for (int s = 0; s < SAMPLES; ++s) {
                double u = (i + rand01()) / (WIDTH - 1.0);
                double v = (j + rand01()) / (HEIGHT - 1.0);
                Ray r = cam.get_ray(u, v);
                col += trace(r, scene, MAX_DEPTH);
            }
            col *= (1.0 / SAMPLES);
            int ir = static_cast<int>(256 * clampd(sqrt(col.x), 0.0, 0.999));
            int ig = static_cast<int>(256 * clampd(sqrt(col.y), 0.0, 0.999));
            int ib = static_cast<int>(256 * clampd(sqrt(col.z), 0.0, 0.999));
            cout << ir << " " << ig << " " << ib << "\n";
        }
    }
  
    cerr << "\nRender complete.\n";
    return 0;
}
