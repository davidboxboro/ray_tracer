#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
public:
    Ray() {}
    Ray(const Point3& orig, const Vec3& dir) 
        : orig(orig), dir(dir) {}

    Point3 origin() const { return orig; }
    Point3 direction() const { return dir; }

    Point3 at(double t) const {
        return orig + t * dir;
    }

private:
    Point3 orig;
    Vec3 dir;
};

#endif