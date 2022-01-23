#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "material.h"

class Material;

struct HitRecord {
    Point3 p;
    Vec3 normal;
    std::shared_ptr<Material> matPtr;
    double t;
    bool frontFace; // whether normal is pointing outward

    void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
        frontFace = (dot(r.direction(), outwardNormal) < 0);
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const = 0;
};

#endif