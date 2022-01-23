#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include "utils.h"

class Vec3 {
public:
    Vec3() : e{0, 0, 0} {}
    Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    Vec3 operator-() const { return {-e[0], -e[1], -e[2]}; }
    double operator[](const int i) const { return e[i]; }
    double& operator[](const int i) { return e[i]; }

    Vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vec3& operator/=(const double t) {
        return (*this) *= (1 / t);
    }

    Vec3& operator+=(const Vec3& v) {
        e[0] += v.x();
        e[1] += v.y();
        e[2] += v.z();
        return *this;
    }

    Vec3& operator-=(const Vec3& v) {
        return (*this) += (-v);
    }

    double lengthSquared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    double length() const {
        return std::sqrt(lengthSquared());
    }

    bool nearZero() const {
        const static double eps = 1e-8;
        return (std::abs(e[0]) < eps) && (std::abs(e[1]) < eps) && (std::abs(e[2]) < eps);
    }

private:
    double e[3];
};


// Vec3 utility functions 
inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return Vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return Vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline Vec3 operator/(const Vec3& u, const Vec3& v) {
    return Vec3(u.x() / v.x(), u.y() / v.y(), u.z() / v.z());
}

inline Vec3 operator*(const Vec3& v, double t) {
    return Vec3(t * v.x(), t * v.y(), t * v.z());
}

inline Vec3 operator*(double t, const Vec3& v) {
    return v * t;
}

inline Vec3 operator/(const Vec3& v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vec3& u, const Vec3& v) {
    return u.x()*v.x() + u.y()*v.y() + u.z()*v.z();
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
    return Vec3(
        u.y()*v.z() - u.z()*v.y(),
        u.z()*v.x() - u.x()*v.z(),
        u.x()*v.y() - u.y()*v.x()
    );
}

inline Vec3 unitVector(const Vec3& v) {
    return v / v.length();
}

inline Vec3 randomVector(double min, double max) {
    return Vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
}

inline Vec3 randomVector() {
    return randomVector(0, 1);
}

inline Vec3 randomInUnitSphere() {
    Vec3 v;
    while (true) {
        v = randomVector(-1, 1);
        if (v.lengthSquared() <= 1) {
            return v;
        }
    }
}

inline Vec3 randomUnitVector() {
    return unitVector(randomInUnitSphere());
}

inline Vec3 randomInUnitDisk() {
    Vec3 v;
    while (true) {
        v = Vec3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
        if (v.lengthSquared() <= 1) {
            return v;
        }
    }
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3& v, const Vec3& n, double etaOverEtaPrime) {
    const double cosTheta = -dot(v, n);
    const Vec3 vPrimePerp = etaOverEtaPrime * (v + cosTheta * n);
    const Vec3 vPrimeParallel = -std::sqrt(1 - vPrimePerp.lengthSquared()) * n;
    return vPrimePerp + vPrimeParallel;
}


// type aliases for Vec3
using Point3 = Vec3;
using Color = Vec3;

#endif