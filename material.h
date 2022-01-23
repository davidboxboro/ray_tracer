#ifndef MATERIAL_H
#define MATERIAL_H

#include "utils.h"
#include "hittable.h"

#include <algorithm>

struct HitRecord;

// Material abstract class 
class Material {
public:
    virtual bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rScattered) const = 0;
};

// Lambertian class
class Lambertian : public Material {
public:
    Lambertian(const Color& albedo) : albedo(albedo) {}
    virtual ~Lambertian() = default;

    virtual bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rScattered) const override;

private:
    Color albedo;
};

// Metal class
class Metal : public Material {
public:
    Metal(const Color& albedo, double fuzz) : albedo(albedo), fuzz(std::min(fuzz, 1.0)) {}
    virtual ~Metal() = default;

    virtual bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rScattered) const override;

private:
    Color albedo;
    double fuzz;
};

// Dielectric class 
class Dielectric : public Material {
public:
    Dielectric(double indexOfRefraction) : indexOfRefraction(indexOfRefraction) {}
    virtual ~Dielectric() = default;

    virtual bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& rScattered) const override;
    static double reflectance(double cosineTheta, double refractionRatio);
private:
    double indexOfRefraction;
};


#endif