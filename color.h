#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "utils.h"

inline void writeColor(std::ostream& out, Color pixelColor, int samplesPerPixel) {
    const static double pixelUpperBound = 256;
    const static double colorUpperBound = 0.9999;

    double r = pixelColor.x();
    double g = pixelColor.y();
    double b = pixelColor.z();

    // square root gamma correction between our eyes do not perceive light linearly
    const double scale = 1.0 / samplesPerPixel;
    r = std::sqrt(r * scale);
    g = std::sqrt(g * scale);
    b = std::sqrt(b * scale);

    out << static_cast<int>(pixelUpperBound * clamp(r, 0, colorUpperBound)) << ' '
        << static_cast<int>(pixelUpperBound * clamp(g, 0, colorUpperBound)) << ' '
        << static_cast<int>(pixelUpperBound * clamp(b, 0, colorUpperBound)) << '\n';
}


#endif