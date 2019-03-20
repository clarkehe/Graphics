//
//  Matrix4x4.cpp
//  Math
//
//  Created by Clarke on 2018/12/20.
//  Copyright © 2018 Clarke. All rights reserved.
//

#include "stdafx.h"
#include "Matrix4x4.hpp"

Matrix4x4::Matrix4x4()
{
    memset(m, 0x00, sizeof(m));
}

Matrix4x4::Matrix4x4(double m00, double m01, double m02, double m03,
                     double m10, double m11, double m12, double m13,
                     double m20, double m21, double m22, double m23,
                     double m30, double m31, double m32, double m33)
{
    this->m00 = m00;
    this->m01 = m01;
    this->m02 = m02;
    this->m03 = m03;
    
    this->m10 = m10;
    this->m11 = m11;
    this->m12 = m12;
    this->m13 = m13;
    
    this->m20 = m20;
    this->m21 = m21;
    this->m22 = m22;
    this->m23 = m23;
    
    this->m30 = m30;
    this->m31 = m31;
    this->m32 = m32;
    this->m33 = m33;
}

Matrix4x4::Matrix4x4(double r0[4], double r1[4], double r2[4], double r3[4])
{
    memcpy(this->r0, r0, sizeof(this->r0));
    memcpy(this->r1, r1, sizeof(this->r1));
    memcpy(this->r2, r2, sizeof(this->r2));
    memcpy(this->r3, r3, sizeof(this->r3));
}

Matrix4x4::Matrix4x4(double m[16])
{
    memcpy(this->m, m, sizeof(this->m));
}

void Matrix4x4::SetIndentity()
{
    memset(m, 0x00, sizeof(m));
    this->m00 = this->m11 = this->m22 = this->m33 = 1;
}

Matrix4x4 Matrix4x4::Transpose() const
{
    Matrix4x4 m;
    
    m.m00 = this->m00;
    m.m01 = this->m10;
    m.m02 = this->m20;
    m.m03 = this->m30;
    
    m.m10 = this->m01;
    m.m11 = this->m11;
    m.m12 = this->m21;
    m.m13 = this->m31;
    
    m.m20 = this->m02;
    m.m21 = this->m12;
    m.m22 = this->m22;
    m.m23 = this->m32;
    
    m.m30 = this->m03;
    m.m31 = this->m13;
    m.m32 = this->m23;
    m.m33 = this->m33;
    
    return  m;
}

Matrix4x4 Matrix4x4::Inverse() const
{
    Matrix4x4 m;
    return m;
}

Matrix4x4 Matrix4x4::operator*(const double scale) const
{
    Matrix4x4 m;
    memcpy(m.m, this->m, sizeof(this->m));
    for (int i=0; i<16; i++)
        m.m[i] *= scale;
    return m;
}

/*
    this * v3
 */
Vector3D Matrix4x4::operator*(const Vector3D &v) const
{
    double x = this->m00 * v.x + this->m01 * v.y + this->m02 * v.z;
    double y = this->m10 * v.x + this->m11 * v.y + this->m12 * v.z;
    double z = this->m20 * v.x + this->m21 * v.y + this->m22 * v.z;
    return Vector3D(x, y, z);
}

/*
    this * v4
 */
Vector4D Matrix4x4::operator*(const Vector4D &v) const
{
    double x = this->m00 * v.x + this->m01 * v.y + this->m02 * v.z + this->m03 * v.w;
    double y = this->m10 * v.x + this->m11 * v.y + this->m12 * v.z + this->m13 * v.w;
    double z = this->m20 * v.x + this->m21 * v.y + this->m22 * v.z + this->m23 * v.w;
    double w = this->m30 * v.x + this->m31 * v.y + this->m32 * v.z + this->m33 * v.w;
    return Vector4D(x, y, z, w);
}

/*
    this * m
 */
Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &m) const
{
    Matrix4x4 result;
    
    result.m00 = this->m00 * m.m00 + this->m01 * m.m10 + this->m02 * m.m20 + this->m03 * m.m30;
    result.m10 = this->m10 * m.m00 + this->m11 * m.m10 + this->m12 * m.m20 + this->m13 * m.m30;
    result.m20 = this->m20 * m.m00 + this->m21 * m.m10 + this->m22 * m.m20 + this->m23 * m.m30;
    result.m30 = this->m30 * m.m00 + this->m31 * m.m10 + this->m32 * m.m20 + this->m33 * m.m30;
    
    result.m01 = this->m00 * m.m01 + this->m01 * m.m11 + this->m02 * m.m21 + this->m03 * m.m31;
    result.m11 = this->m10 * m.m01 + this->m11 * m.m11 + this->m12 * m.m21 + this->m13 * m.m31;
    result.m21 = this->m20 * m.m01 + this->m21 * m.m11 + this->m22 * m.m21 + this->m23 * m.m31;
    result.m31 = this->m30 * m.m01 + this->m31 * m.m11 + this->m32 * m.m21 + this->m33 * m.m31;

    result.m02 = this->m00 * m.m02 + this->m01 * m.m12 + this->m02 * m.m22 + this->m03 * m.m32;
    result.m12 = this->m10 * m.m02 + this->m11 * m.m12 + this->m12 * m.m22 + this->m13 * m.m32;
    result.m22 = this->m20 * m.m02 + this->m21 * m.m12 + this->m22 * m.m22 + this->m23 * m.m32;
    result.m32 = this->m30 * m.m02 + this->m31 * m.m12 + this->m32 * m.m22 + this->m33 * m.m32;
    
    result.m03 = this->m00 * m.m03 + this->m01 * m.m13 + this->m02 * m.m23 + this->m03 * m.m33;
    result.m13 = this->m10 * m.m03 + this->m11 * m.m13 + this->m12 * m.m23 + this->m13 * m.m33;
    result.m23 = this->m20 * m.m03 + this->m21 * m.m13 + this->m22 * m.m23 + this->m23 * m.m33;
    result.m33 = this->m30 * m.m03 + this->m31 * m.m13 + this->m32 * m.m23 + this->m33 * m.m33;
    
    return result;
}

Matrix4x4 Matrix4x4::RotateAroundZ(double angle)
{
    Matrix4x4 m;
    m.SetIndentity();
    
    //x, (1, 0, 0) -> (cos, sin, 0)
    m.m00 = cos(angle);
    m.m10 = sin(angle);
    m.m20 = 0;
    m.m30 = 0;
    
    //y, (0, 1, 0) -> (-sin, cos, 0)
    m.m01 = -sin(angle);
    m.m11 = cos(angle);
    m.m21 = 0;
    m.m31 = 0;
    
    //z, (0, 0, 1), keep the same
    return m;
}

Matrix4x4 Matrix4x4::RotateAroundY(double angle)
{
    Matrix4x4 m;
    m.SetIndentity();
    
    //x, (1, 0, 0) -> (cos, 0, -sin)
    m.m00 = cos(angle);
    m.m10 = 0;
    m.m20 = -sin(angle);
    m.m30 = 0;
    
    //y, (0, 1, 0) keep the same
    
    //z, (0, 0, 1) -> (sin, 0, cos0)
    m.m02 = sin(angle);
    m.m12 = 0;
    m.m22 = cos(angle);
    m.m32 = 0;
    
    return m;
}

Matrix4x4 Matrix4x4::RotateAroundX(double angle)
{
    Matrix4x4 m;
    m.SetIndentity();
    
    //x, (1, 0, 0) keep the same
    
    //y, (0, 1, 0) -> (0, cos, sin)
    m.m01 = 0;
    m.m11 = cos(angle);
    m.m21 = sin(angle);
    m.m31 = 0;
    
    //z, (0, 0, 1) -> (0, -sin, cos)
    m.m02 = 0;
    m.m12 = -sin(angle);
    m.m22 = cos(angle);
    m.m32 = 0;
    
    return m;
}

Matrix4x4 Matrix4x4::RoateAroundVector(Vector3D &vector, double angle)
{
    Matrix4x4 m;
    m.SetIndentity();
    return m;
}

Matrix4x4 Matrix4x4::Scale(Vector3D &scale)
{
    Matrix4x4 m;
    m.SetIndentity();
    m.m00 = scale.x;
    m.m11 = scale.y;
    m.m22 = scale.z;
    return m;
}

//Scale -> Projection -> Mirror
Matrix4x4 Matrix4x4::Scale(Vector3D &vector, double scale)
{
    Matrix4x4 m;
    m.SetIndentity();
    return m;
}

Matrix4x4 Matrix4x4::Translate(const Vector3D &vector)
{
    Matrix4x4 m;
    m.SetIndentity();
    m.m03 = vector.x;
    m.m13 = vector.y;
    m.m23 = vector.z;
    return m;
}

std::ostream & operator << (std::ostream &out, const Matrix4x4 &m)
{
    out << m.m00 << "," << m.m01 << "," << m.m02 << "," << m.m03 << std::endl;
    out << m.m10 << "," << m.m11 << "," << m.m12 << "," << m.m13 << std::endl;
    out << m.m20 << "," << m.m21 << "," << m.m22 << "," << m.m23 << std::endl;
    out << m.m30 << "," << m.m31 << "," << m.m32 << "," << m.m33 << std::endl;
    return out;
}
