#pragma once
#include <cmath>
#include <ostream>

namespace simcore::math {

struct Vec3 {
    double x;
    double y;
    double z;

    // --- Constructors ---
    constexpr Vec3() : x(0.0), y(0.0), z(0.0) {}
    constexpr Vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

    // --- Basic arithmetic ---
    constexpr Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }

    constexpr Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }

    constexpr Vec3 operator*(double s) const { return {x * s, y * s, z * s}; }

    constexpr Vec3 operator/(double s) const { return {x / s, y / s, z / s}; }

    Vec3& operator+=(const Vec3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vec3& operator*=(double s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    Vec3& operator/=(double s) {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    // --- Norms ---
    double norm() const { return std::sqrt(x * x + y * y + z * z); }

    double norm_squared() const { return x * x + y * y + z * z; }

    Vec3 normalized() const {
        double n = norm();
        return (n > 0.0) ? (*this / n) : Vec3{};
    }

    // --- Static helpers ---
    static double dot(const Vec3& a, const Vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

    static Vec3 cross(const Vec3& a, const Vec3& b) {
        return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    }
};

// scalar * vector
inline constexpr Vec3 operator*(double s, const Vec3& v) {
    return v * s;
}

// stream printing (useful for debugging/tests)
inline std::ostream& operator<<(std::ostream& os, const Vec3& v) {
    os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return os;
}

} // namespace simcore::math
