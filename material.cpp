#include "material.h"

bool Lambertian::scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rScattered) const {
    Vec3 scatterDir = rec.normal + randomUnitVector();
    // catch degenerate case
    if (scatterDir.nearZero()) {
        scatterDir = rec.normal;
    }
    rScattered = Ray(rec.p, scatterDir);
    attenuation = albedo;
    return true;
}


bool Metal::scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rScattered) const {
    const Vec3 reflectedDir = reflect(unitVector(rIn.direction()), rec.normal);
    rScattered = Ray(rec.p, reflectedDir + fuzz * randomInUnitSphere());
    attenuation = albedo;
    const bool scatteredOutside = dot(rScattered.direction(), rec.normal) > 0;
    return scatteredOutside;
}


bool Dielectric::scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rScattered) const {
    attenuation = Color(1, 1, 1);
    const double refractionRatio = rec.frontFace ? (1.0 / indexOfRefraction) : indexOfRefraction;
    const Vec3 unitDirection = unitVector(rIn.direction());

    // either refract or reflect 
    const double cosTheta = -dot(unitDirection, rec.normal);
    const double sinTheta = std::sqrt(1 - cosTheta*cosTheta);
    const bool cannotRefract = (refractionRatio * sinTheta) > 1;
    Vec3 newDirection;
    if (cannotRefract || reflectance(cosTheta, refractionRatio) > randomDouble()) {
        newDirection = reflect(unitDirection, rec.normal);
    } else {
        newDirection = refract(unitDirection, rec.normal, refractionRatio);
    }
    rScattered = Ray(rec.p, newDirection);
    return true;
}

double Dielectric::reflectance(double cosineTheta, double refractionRatio) {
    // Use Schlick's approximation for reflectance.
    double r0 = (1 - refractionRatio) / (1 + refractionRatio);
    r0 *= r0;
    return r0 + (1 - r0) * pow(1 - cosineTheta, 5);
}