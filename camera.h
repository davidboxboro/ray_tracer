#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"
#include "ray.h"

class Camera {
public:
    Camera(Point3 lookFrom, Point3 lookAt, Vec3 vUp, double vertFieldOfView, 
      double aspectRatio, double aperture, double focusDist) {

        const double theta = degreesToRadians(vertFieldOfView);
        const double h = tan(theta / 2);
        const double viewportHeight = 2.0 * h;
        const double viewPortWidth = aspectRatio * viewportHeight;

        w = unitVector(lookFrom - lookAt);
        u = unitVector(cross(vUp, w));
        v = unitVector(cross(w, u));

        origin = lookFrom;
        horizontal = focusDist * viewPortWidth * u;
        vertical = focusDist * viewportHeight * v;
        lowerLeftCorner = origin - horizontal/2 - vertical/2 - focusDist * w;

        lensRadius = aperture / 2;
    }

    Ray getRay(double s, double t) const {
        Vec3 random = lensRadius * randomInUnitDisk();
        Vec3 offset = u * random.x() + v * random.y();
        return Ray(origin + offset, lowerLeftCorner + s*horizontal + t*vertical - origin - offset);
    }

private:
    Point3 origin;
    Point3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    double lensRadius;
};


#endif