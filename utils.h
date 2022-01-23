#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <random>

// global constants 
const double INF = std::numeric_limits<double>::infinity();
const double PI = M_PI;

// utility functions
inline double degreesToRadians(double degrees) {
    return degrees * PI / 180.0;
}

inline double randomDouble() {
    static std::uniform_real_distribution<double> distribution(0, 1);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double randomDouble(double min, double max) {
    return min + (max - min) * randomDouble();
}

inline double clamp(double x, double min, double max) {
    if (x < min) { return min; }
    if (x > max) { return max; }
    return x;
}

#endif