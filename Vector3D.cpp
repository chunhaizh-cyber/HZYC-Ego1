#include "Vector3D.h"
#include <cmath>

Vector3D::Vector3D() : x(0), y(0), z(0) {}

Vector3D::Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

Vector3D Vector3D::operator+(const Vector3D& other) const {
    return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::operator-(const Vector3D& other) const {
    return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D Vector3D::operator*(double scalar) const {
    return Vector3D(x * scalar, y * scalar, z * scalar);
}

Vector3D Vector3D::operator/(double scalar) const {
    if (scalar != 0) {
        return Vector3D(x / scalar, y / scalar, z / scalar);
    }
    return Vector3D(0, 0, 0);
}

double Vector3D::distance(const Vector3D& other) const {
    double dx = x - other.x;
    double dy = y - other.y;
    double dz = z - other.z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

double Vector3D::magnitude() const {
    return std::sqrt(x*x + y*y + z*z);
}

Vector3D Vector3D::normalized() const {
    double mag = magnitude();
    if (mag > 0) {
        return Vector3D(x/mag, y/mag, z/mag);
    }
    return Vector3D(0, 0, 0);
}

std::ostream& operator<<(std::ostream& os, const Vector3D& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}