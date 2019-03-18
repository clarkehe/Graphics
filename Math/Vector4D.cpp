//
//  Vector4D.cpp
//  Math
//
//  Created by Clarke on 2018/12/25.
//  Copyright © 2018 Clarke. All rights reserved.
//

#include "Vector4D.hpp"
#include "Vector3D.hpp"

Vector4D::Vector4D(double x, double y, double z, double w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector4D::Vector4D(const Vector3D &v, double w)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->w = w;
}

Vector3D Vector4D::ToVector3D() const
{
    return Vector3D(x, y, z);
}

ostream & operator << (ostream &out, const Vector4D &v)
{
    out << v.x << "," << v.y << "," << v.z << "," << v.w << endl;
    return out;
}
