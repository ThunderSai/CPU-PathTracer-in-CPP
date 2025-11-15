#include "../include/aabb.h"
#include <algorithm>
#include <cmath>
// funtion to create a box around by defining its corners as ( x,y,z ) coordinates and check whether the ray hits the box or not in the given range ( tmin , tmax )  
bool AABB::hit(const Ray &r, double tmin, double tmax) const {
    for (int a = 0; a < 3; ++a) {                              //
        double invD;                                           // 
        double originComp, dirComp, minComp, maxComp;          //
        if (a==0){                                             //
            originComp = r.o.x;                                //
            dirComp = r.d.x;                                   //
            minComp = minimum.x;                               //
            maxComp = maximum.x;                               //
                                                               // loop over the axis to select the correct axis to pick according to the loop  
        }                                                      //       
        else if (a==1){                                        //
            originComp = r.o.y;                                //
            dirComp = r.d.y;                                   //
            minComp = minimum.y;                               //
            maxComp = maximum.y;                               //
        }                                                      //
        else {                                                 //
            originComp = r.o.z;                                //
            dirComp = r.d.z;                                   //
            minComp = minimum.z;                               //
            maxComp = maximum.z;                               //   
        }                                                      //
        if (fabs(dirComp) < 1e-12) {                              //
            if (originComp < minComp || originComp > maxComp){    //
                return false;                                     // --> to handle the case in which the ray is parellel to the plane and also to check the position of the origin that whther it is outside 
            }                                                     //      the salb or inside the slab 
        }                                                         //
            // if the ray hits the box and it's direction is non - zero 
        else {   
            // distances at which the ray intersects the two planes 
            invD = 1.0 / dirComp;
            double t0 = (minComp - originComp) * invD;
            double t1 = (maxComp - originComp) * invD;
            // if the direction is negative then t0 and t1 would be swapped 
            if (invD < 0.0) {
                std::swap(t0,t1);
            }
            // to update the global variables
            tmin = t0 > tmin ? t0 : tmin;
            tmax = t1 < tmax ? t1 : tmax;
            if (tmax <= tmin) {
                return false;
            }
        }
    }

    return true;
}
// constructor to two other AABB 
AABB surrounding_box(const AABB &box0, const AABB &box1) {
    Vec3 small( std::min(box0.min().x, box1.min().x),
                std::min(box0.min().y, box1.min().y),
                std::min(box0.min().z, box1.min().z) );
                
    Vec3 big(   std::max(box0.max().x, box1.max().x),
                std::max(box0.max().y, box1.max().y),
                std::max(box0.max().z, box1.max().z) );

    return AABB(small, big);
}
