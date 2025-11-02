#pragma once
#include "hittable.h"
#include <vector>
using namespace std;

struct Scene: public Hittable {
    vector<shared_ptr<Hittable>> objs;

    void add(shared_ptr<Hittable> h){ 
        objs.push_back(h); 
    }

    bool hit(const Ray &r, double tmin, double tmax, HitRecord &rec) const override {
        HitRecord temp; 
        bool hit_any=false; 
        double closest=tmax;

        for (const auto &o: objs){
            if (o->hit(r, tmin, closest, temp)){
                hit_any = true;
                closest = temp.t;
                rec = temp;
            }
        }

        return hit_any;
    }
};
