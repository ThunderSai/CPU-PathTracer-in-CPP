# CPU-PathTracer-in-CPP
A CPU-based path tracing renderer built in C++ from scratch. This project is part of my DSA learning journey, where I explore data structures, algorithms, and numerical methods through realistic light simulation and rendering techniques.

## 📁 Project Structure

/with_bvh/

### 🧩 Header files
├── vec3.h <br>
├── ray.h <br>
├── camera.h <br>
├── hittable.h <br>
├── material.h <br>
├── aabb.h <br>
└── utils.h <br>

### ⚙️ Source files
├── main.cpp <br>
├── vec3.cpp <br>
├── camera.cpp <br>
├── hittable.cpp <br>
├── material.cpp <br>
└── aabb.cpp <br>

Other files

├── Makefile  # Build instructions <br>

Build Instructions

To build the project, simply run:
```cpp
make with_bvh
```

if make is not available:

```cpp
g++ -std=c++11 -O3 -fopenmp main.cpp vec3.cpp aabb.cpp material.cpp hittable.cpp camera.cpp -o ray_tracer
./ray_tracer > image1.ppm
```

## 🧠 Key Concepts

### Path Tracing
Path tracing is a physically accurate rendering algorithm that simulates the random scattering of light rays in a scene to compute color and illumination.

### Bounding Volume Hierarchy (BVH)
BVH partitions scene geometry into bounding boxes, allowing the renderer to skip large portions of the scene that cannot be intersected by a ray — significantly reducing render time for complex scenes.

### Parallelization
OpenMP is used to parallelize ray calculations across CPU cores, taking advantage of multi-core architectures.


