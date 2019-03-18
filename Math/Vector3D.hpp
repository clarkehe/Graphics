//
//  Vector3D.hpp
//  Math
//
//  Created by Clarke on 2018/12/18.
//  Copyright © 2018 Clarke. All rights reserved.
//

#ifndef Vector3D_hpp
#define Vector3D_hpp

#include <stdio.h>
#include <iostream>
using namespace std;

class Vector3D
{
    friend ostream & operator << (ostream &out, const Vector3D &v);

public:
    double x, y, z;
    
public:
    explicit Vector3D();
    explicit Vector3D(double v);
    explicit Vector3D(double x, double y, double z);
    
public:
    double LengthSquare() const;
    double Length() const;
    void Normalize();
    
    double Dot(const Vector3D &vector) const;
    Vector3D Cross(const Vector3D &vector) const;
    
    Vector3D operator-() const;
    Vector3D operator+(const Vector3D &vector) const;
    Vector3D operator*(const double scale) const;
    
    bool operator==(const Vector3D &vector);
    
    double AngleWith(const Vector3D &vector) const;
};

ostream & operator << (ostream &out, const Vector3D &v);

#endif /* Vector3D_hpp */
