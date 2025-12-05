#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
#include <iostream>

class Vector3D {
public:
    double x, y, z;
    
    Vector3D();
    Vector3D(double x, double y, double z);
    
    Vector3D operator+(const Vector3D& other) const;
    Vector3D operator-(const Vector3D& other) const;
    Vector3D operator*(double scalar) const;
    Vector3D operator/(double scalar) const;
    
    double distance(const Vector3D& other) const;
    double magnitude() const;
    Vector3D normalized() const;
    
    friend std::ostream& operator<<(std::ostream& os, const Vector3D& vec);
};

#endif // VECTOR3D_H