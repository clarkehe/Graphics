//
//  Matrix4x4.hpp
//  Math
//
//  Created by Clarke on 2018/12/20.
//  Copyright © 2018 Clarke. All rights reserved.
//

#ifndef Matrix4x4_hpp
#define Matrix4x4_hpp

#include "Vector3D.hpp"
#include "Vector4D.hpp"

class Matrix4x4
{
public:
    union
    {
        struct
        {
            double m00, m01, m02, m03;
            double m10, m11, m12, m13;
            double m20, m21, m22, m23;
            double m30, m31, m32, m33;
        };
        
        struct
        {
            double r0[4];
            double r1[4];
            double r2[4];
            double r3[4];
        };
        
        double m[16];
    };
    
public:
    explicit Matrix4x4();
    
    explicit Matrix4x4(double m00, double m01, double m02, double m03,
                       double m10, double m11, double m12, double m13,
                       double m20, double m21, double m22, double m23,
                       double m30, double m31, double m32, double m33);
    
    explicit Matrix4x4(double r0[4], double r1[4], double r2[4], double r3[4]);
    
    explicit Matrix4x4(double m[16]);
    
    void SetIndentity();
    
    Matrix4x4 Transpose() const;
    Matrix4x4 Inverse() const;
    
    Matrix4x4 operator*(const double scale) const;
    Vector3D  operator*(const Vector3D &v) const;
    Vector4D  operator*(const Vector4D &v) const;
    Matrix4x4 operator*(const Matrix4x4 &m) const;
    
    static Matrix4x4 RotateAroundX(double angle);
    static Matrix4x4 RotateAroundY(double angle);
    static Matrix4x4 RotateAroundZ(double angle);
    
    static Matrix4x4 RoateAroundVector(Vector3D &vector, double angle);
    
    static Matrix4x4 Scale(Vector3D &scale);
    static Matrix4x4 Scale(Vector3D &vector, double scale);
    
    static Matrix4x4 Translate(const Vector3D &vector);
};

std::ostream & operator << (std::ostream &out, const Matrix4x4 &m);

#endif /* Matrix4x4_hpp */
