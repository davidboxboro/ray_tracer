#include <iostream>
#include "utils.h"
#include "hittable_list.h"
#include "color.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

HittableList sickView() {
    HittableList world;

    const double smallRadius = 0.05;
    const double mediumRadius = 0.2;
    const double largeRadius = 1;
    
    std::vector<Point3> centers = {
        Point3(0, largeRadius, 0), 
        Point3(-4 * largeRadius, largeRadius, 0),
        Point3(4 * largeRadius, largeRadius, 0)
    };

    auto notIntersectOthers = [&centers](const Point3& center) {
        double radius = center[1];
        for (const Point3& otherCenter : centers) {
            double otherRadius = otherCenter[1];
            if ((otherCenter - center).length() < radius + otherRadius) {
                return false;
            }
        }
        return true;
    };

    auto groundMaterial = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

    auto makeSpheres = [&](double radius, double stepSize) {
        for (double a = -11; a < 11; a += stepSize) {
            for (double b = -11; b < 11; b += stepSize) {
                double chooseMaterial = randomDouble();
                Point3 center(a + randomDouble(), radius, b + randomDouble());
                if (notIntersectOthers(center)) {
                    centers.push_back(center);
                    std::shared_ptr<Material> sphereMaterial;
                    if (chooseMaterial < 0.6) {
                        // diffuse
                        Color albedo = randomVector();
                        sphereMaterial = std::make_shared<Lambertian>(albedo);
                        world.add(std::make_shared<Sphere>(center, radius, sphereMaterial));
                    } else if (chooseMaterial < 0.8) {
                        // metal
                        Color albedo = randomVector(0.5, 1);
                        double fuzz = randomDouble(0, 0.5);
                        sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
                        world.add(std::make_shared<Sphere>(center, radius, sphereMaterial));
                    } else {
                        // glass
                        double refractIdx = randomDouble(1.1, 2);
                        sphereMaterial = std::make_shared<Dielectric>(refractIdx);
                        world.add(std::make_shared<Sphere>(center, radius, sphereMaterial));
                    }
                }
            }
        }
    };

    makeSpheres(mediumRadius, 1);
    makeSpheres(smallRadius, 0.4);

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(centers[0], largeRadius, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.2, 0.3, 0.4));
    world.add(std::make_shared<Sphere>(centers[1], largeRadius, material2));

    auto material3 = std::make_shared<Metal>(Color(0.9, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(centers[2], largeRadius, material3));

    return world;
}


Color rayColor(const Ray& r, const Hittable& world, int depth) {
    HitRecord rec;
    // if exceeded ray bounce limit, no more light gathered
    if (depth <= 0) {
        return Color(0, 0, 0);
    }

    const static float tolerance = 0.001;
    if (world.hit(r, tolerance, INF, rec)) {
        Ray rScattered;
        Color attenuation;
        if (rec.matPtr->scatter(r, rec, attenuation, rScattered)) {
            return attenuation * rayColor(rScattered, world, depth - 1);
        }
        return Color(0, 0, 0);
    }
    Vec3 unitDirection = unitVector(r.direction());
    double t = 0.5 * (unitDirection.y() + 1);
    return (1 - t) * Color(1, 1, 1) + t * Color(0.5, 0.7, 1);
}

int main() {
    // image 
    const double aspectRatio = 3.0 / 2;
    const int imageWidth = 1000;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 50;
    const int maxDepth = 50;

    // world
    HittableList world = sickView();

    // camera
    Point3 lookFrom(13, 2, 3);
    Point3 lookAt(0, 0, 0);
    Point3 vUp(0, 1, 0);
    double vertFieldOfView = 20;
    double aperture = 0.1;
    double distToFocus = 10;

    Camera camera(lookFrom, lookAt, vUp, vertFieldOfView, aspectRatio, aperture, distToFocus);

    // render
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
    for (int j = imageHeight-1; j >= 0; j--) {
        std::cerr << "\rLines remaining: " << j << ' ';
        for (int i = 0; i < imageWidth; i++) {
            Color pixelColor(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; s++) {
                double u = (i + randomDouble()) / (imageWidth-1); // add Uniform[0, 1) noise to i
                double v = (j + randomDouble()) / (imageHeight-1);
                Ray r = camera.getRay(u, v);
                pixelColor += rayColor(r, world, maxDepth);
            }
            writeColor(std::cout, pixelColor, samplesPerPixel);
        }
    }
    std::cerr << "\nDone.\n";
}