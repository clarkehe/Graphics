//
//  Vector3D.cpp
//  Math
//
//  Created by Clarke on 2018/12/18.
//  Copyright © 2018 Clarke. All rights reserved.
//

#include "stdafx.h"
#include "Vector3D.hpp"

Vector3D::Vector3D()
{
    x = y = z = 0;
}

Vector3D::Vector3D(double v)
{
    x = y = z = v;
}

Vector3D::Vector3D(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

double Vector3D::LengthSquare() const
{
    return x * x + y * y + z * z;
}

double Vector3D::Length() const
{
    return sqrt(LengthSquare());
}

Vector3D& Vector3D::Normalize()
{
    const double len = Length();
    if (len <= std::numeric_limits<double>::epsilon())
        return *this;
    
    this->x /= len;
    this->y /= len;
    this->z /= len;
    
    return *this;
}

double Vector3D::Dot(const Vector3D &vector) const
{
    return this->x * vector.x + this->y * vector.y + this->z * vector.z;
}

/*
    x x
    y y
    z z
 */
Vector3D Vector3D::Cross(const Vector3D &vector) const
{
    return Vector3D( this->y * vector.z - this->z * vector.y,
                    -this->x * vector.z + this->z * vector.x,
                     this->x * vector.y - this->y * vector.x);
}

Vector3D Vector3D::operator*(const double scale) const
{
    return Vector3D(x * scale, y * scale, z * scale);
}

Vector3D Vector3D::operator+(const Vector3D &vector) const
{
    return Vector3D(x + vector.x, y + vector.y, z + vector.z);
}

Vector3D Vector3D::operator-(const Vector3D &vector) const
{
    return Vector3D(x - vector.x, y - vector.y, z - vector.z);
}

Vector3D Vector3D::operator-() const
{
    return Vector3D(-x, -y, -z);
}

double Vector3D::AngleWith(const Vector3D &vector) const
{
    double cosValue = this->Dot(vector) / (this->Length() * vector.Length());
    return acos(cosValue);
}

bool Vector3D::operator==(const Vector3D &vector)
{
    const double epsilon = std::numeric_limits<double>::epsilon();

    const double deltX = abs(this->x - vector.x);
    const double deltY = abs(this->y - vector.y);
    const double deltZ = abs(this->z - vector.z);
    
    return deltX < epsilon && deltY < epsilon && deltZ < epsilon;
}

std::ostream & operator << (std::ostream &out, const Vector3D &v)
{
    out << v.x << "," << v.y << "," << v.z << std::endl;
    return out;
}
