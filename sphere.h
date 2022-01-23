#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "memory"

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(const Point3& center, double radius, std::shared_ptr<Material> matPtr)
        : center(center), radius(radius), matPtr(matPtr) {}
    virtual ~Sphere() = default;

    virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;

private:
    Point3 center;
    double radius;
    std::shared_ptr<Material> matPtr;
};

inline bool Sphere::hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
    Vec3 oc = r.origin() - center;
    const double a = r.direction().lengthSquared();
    const double halfB = dot(r.direction(), oc);
    const double c = oc.lengthSquared() - radius*radius;
    const double discrim = halfB*halfB - a*c;
    if (discrim < 0) {
        return false;
    }
    const double sqrtDiscrim = std::sqrt(discrim);
    // find nearest root that lies in acceptable range
    double root = (-halfB - sqrtDiscrim) / a;
    if (root < tMin || root > tMax) {
        root = (-halfB + sqrtDiscrim) / a;
        if (root < tMin || root > tMax) {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    const Vec3 outwardNormal = (rec.p - center) / radius;
    rec.setFaceNormal(r, outwardNormal);
    rec.matPtr = matPtr;
    return true;
}

#endif