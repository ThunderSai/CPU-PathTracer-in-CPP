#include "../include/hittable.h"
#include "../include/utils.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <cmath>
// function to check whether the ray(r) hits the sphere in the given range ( tmin to tmax ) or not 
bool Sphere::hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const {
    Vec3 oc = r.o - center; // line to compute the vector oc from the center( center of the sphere ) to the origin of the ray ( r. o ) 
    // constants a , b , c came after simplifying the quadratic eqaution that came from solving the inetrsection of sphere and the ray 
    // basically we can say that these are the quadratic coefficients for intersection of sphere and the ray 
    double a = dot(r.d, r.d);
    double half_b = dot(oc, r.d); // instead of b the term b_half is taken for better and simple calculation
    double c = dot(oc,oc) - radius*radius;
    // code to calculate and the discriminent and check whether the ray hits the sphere or not 
    double disc = half_b*half_b - a*c;
    if (disc <= 0) return false; // if disc < 0 the ray did not hit the sphere and if it id (>0) then it means ray hits the sphere and there exists atleast one solution 
    // code to calculate roots 
    double sq = sqrt(disc);
    double root = (-half_b - sq)/a;
    // we would prefer the smaller root as the smaller would be the root = closure to the sphere 
    // but if that root is not in the given range then we would check for the large root or farther root 
    if (root < tmin || root > tmax) {
        root = (-half_b + sq)/a;
        if (root < tmin || root > tmax) {
            return false;
        }
    }
    rec.t = root; // intersection distance measured along the ray 
    rec.p = r.at(rec.t); // point of intersection 
    Vec3 outward = (rec.p - center) / radius; // calculation for the normal to the surface 
    rec.set_face_normal(r, outward); // to ensure that the normal is anti - parallel to the ray or in the opposite direction of the ray 
    rec.mat = mat; // stores pointer for shading purpose of the sphere 
    return true;
}
// creates a boundary ( AABB ) around the sphere where the min. point would be first term in AABB and the max. would be the second term in AABB   
bool Sphere::bounding_box(double, double, AABB &box) const {
    box = AABB(center - Vec3(radius, radius, radius), center + Vec3(radius, radius, radius));
    return true;
}
// represents a rectangular plane ( finite ) over an infinite plane 
// initializer for the rectangular plane geomatry where the terms used in it are discussed below 
// origin ( refrence point that can be center or any corner ) , u_dir and v_dir are the direction vector along the two sides , u and v ( max , min ) are the limits for the rectangle  
RectPlane::RectPlane(const Vec3 &o, const Vec3 &n, const Vec3 &ud, const Vec3 &vd, 
                     double umin_, double umax_, double vmin_, double vmax_, std::shared_ptr<Material> m)
    : origin(o), normal(n.normalized()), u_dir(ud.normalized()), v_dir(vd.normalized()), 
      umin(umin_), umax(umax_), vmin(vmin_), vmax(vmax_), mat(m) {}
// function that check if the ray hits the rectangular plane or not 
bool RectPlane::hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const {
    // code to find the point of intersection 
    double denom = dot(r.d, normal);
    if (fabs(denom) < 1e-8) {
        return false;
    }
    double t = dot(origin - r.o, normal) / denom;
    // check for intersection point that the intersection point lies in the given range or not 
    if (t < tmin || t > tmax){
        return false;
    }
    
    Vec3 p = r.at(t);                                   //
    Vec3 rel = p - origin;                              //
    double u = dot(rel, u_dir);                         // --> checks if the intersection pointt lies in the bound or not , if the coordinates lies outside the bound then ray hits outside the rectangle  
    double v = dot(rel, v_dir);                         //
    if (u < umin || u > umax || v < vmin || v > vmax){  //
         return false;
    }
                                        
    rec.t = t;                         //
    rec.p = p;                         //
    rec.set_face_normal(r, normal);    // --> stores hit distance fron the ray origin , intersection point , normal and the material 
    rec.mat = mat;                     // 
    return true;                       //
}
// function to build the box around the sphere by making 4 rectangles using the points with x , y, z coordinates 
bool RectPlane::bounding_box(double, double, AABB &box) const {
    Vec3 corners[4];
    corners[0] = origin + umin*u_dir + vmin*v_dir;
    corners[1] = origin + umin*u_dir + vmax*v_dir;
    corners[2] = origin + umax*u_dir + vmin*v_dir;
    corners[3] = origin + umax*u_dir + vmax*v_dir;
    Vec3 minp = corners[0], maxp = corners[0];
    for (int i=1;i<4;++i){
        minp.x = std::min(minp.x, corners[i].x);
        minp.y = std::min(minp.y, corners[i].y);
        minp.z = std::min(minp.z, corners[i].z);
        maxp.x = std::max(maxp.x, corners[i].x);
        maxp.y = std::max(maxp.y, corners[i].y);
        maxp.z = std::max(maxp.z, corners[i].z);
    }
    Vec3 t = normal * EPS_THICK; // provides some thickness along the normal to avoid the zero volume box 
    minp = minp - t;
    maxp = maxp + t;
    box = AABB(minp, maxp);
    return true;
}

BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end) {
    // Create a random number generator (used to pick a random axis: x, y, or z)
    std::random_device rd;
    std::default_random_engine rng(rd());

    // Pick a random axis: 0 = x, 1 = y, 2 = z
    int axis = int(3 * rand01());
    // Choose which comparison function to use depending on the axis
    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;
    
    // Number of objects in this part of the list
    size_t object_span = end - start;
    // CASE 1: Only one object → both children point to the same object
    if (object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {     // CASE 2: Two objects → simply compare and assign left/right
        if (comparator(objects[start], objects[start+1])) {
            left = objects[start];
            right = objects[start+1];
        } else { 
            left = objects[start+1];
            right = objects[start];
        }
    } else {                          // CASE 3: More than two objects → split the list into two halves
        // Sort objects along the chosen axis (x or y or z)
        std::sort(objects.begin() + start, objects.begin() + end, comparator);
        // Middle index for dividing the list
        size_t mid = start + object_span / 2;

        // Recursively build left and right BVH nodes
        left = std::make_shared<BVHNode>(objects, start, mid);
        right = std::make_shared<BVHNode>(objects, mid, end);
    }

    AABB box_left, box_right;  // Get the bounding boxes of both children
    if (!left->bounding_box(0,0, box_left) || !right->bounding_box(0,0, box_right))
        std::cerr << "No bounding box in BVHNode constructor.\n";

    // The bounding box of this node is the box that contains both child boxes
    box = surrounding_box(box_left, box_right);
}

bool BVHNode::hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const {
    // If the ray does NOT hit this node's bounding box, then it cannot hit anything inside it
    if (!box.hit(r, tmin, tmax)) return false;
    HitRecord leftRec, rightRec;
    
    // Try hitting the left child
    // then try hitting the right child
    // NOTE: If left side was hit, we limit tmax to the left hit distance
    // This helps us choose the closest hit
    bool hit_left = left->hit(r, tmin, tmax, leftRec);
    bool hit_right = right->hit(r, tmin, hit_left ? leftRec.t : tmax, rightRec);
    // If right side is hit, return that (right is closer OR left not hit)
    if (hit_right) {
        rec = rightRec;
        return true;
    }
    // If only left side is hit, return that
    if (hit_left) {
        rec = leftRec;
        return true;
    }
                   
    return false; // Nothing hit in this BVH node
}

bool BVHNode::bounding_box(double, double, AABB &out_box) const {
    out_box = box;  // Simply give back the stored bounding box
    return true;
}

bool BVHNode::box_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis) {
    AABB box_a, box_b;
    if (!a->bounding_box(0,0,box_a) || !b->bounding_box(0,0,box_b))    // Each object must have a bounding box
        std::cerr << "No bounding box in box_compare.\n";

    // Compare the minimum corner of each box along the chosen axis
    if (axis == 0) return box_a.min().x < box_b.min().x;  //compare on x 
    else if (axis == 1) return box_a.min().y < box_b.min().y; //compare on y
    else return box_a.min().z < box_b.min().z; //compare on z
}

bool BVHNode::box_x_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) { 
    return box_compare(a,b,0); // Compare using x axis
}

bool BVHNode::box_y_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) { 
    return box_compare(a,b,1); // Compare using y axis
}

bool BVHNode::box_z_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) { 
    return box_compare(a,b,2); // Compare using z axis
}
