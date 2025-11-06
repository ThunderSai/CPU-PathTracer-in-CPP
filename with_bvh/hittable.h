#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "aabb.h"
#include "material.h"
#include <memory>
#include <vector>

struct HitRecord {
    Vec3 p;
    Vec3 normal;
    std::shared_ptr<Material> mat;
    double t;
    bool front_face;
    void set_face_normal(const Ray &r, const Vec3 &outward) {
        front_face = dot(r.d, outward) < 0;
        normal = front_face ? outward : -outward;
    }
};

struct Hittable {
    virtual bool hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const = 0;
    virtual bool bounding_box(double time0, double time1, AABB &box) const = 0;
    virtual ~Hittable() = default;
};

struct Sphere: public Hittable {
    Vec3 center; double radius; std::shared_ptr<Material> mat;
    Sphere(const Vec3 &c, double r, std::shared_ptr<Material> m): center(c), radius(r), mat(m) {}
    bool hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const override;
    bool bounding_box(double, double, AABB &box) const override;
};

struct RectPlane: public Hittable {
    Vec3 origin; Vec3 normal; Vec3 u_dir, v_dir;
    double umin, umax, vmin, vmax;
    std::shared_ptr<Material> mat;
    static constexpr double EPS_THICK = 1e-4;
    RectPlane(const Vec3 &o, const Vec3 &n, const Vec3 &ud, const Vec3 &vd, 
              double umin_, double umax_, double vmin_, double vmax_, std::shared_ptr<Material> m);
    bool hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const override;
    bool bounding_box(double, double, AABB &box) const override;
};

class BVHNode : public Hittable {
public:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB box;

    BVHNode() {}
    BVHNode(std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end);

    virtual bool hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const override;
    virtual bool bounding_box(double, double, AABB &out_box) const override;

private:
    static bool box_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis);
    static bool box_x_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
    static bool box_y_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
    static bool box_z_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
};

#endif
