#pragma once

//Book: 3D 数学基础
//LeftHand: Change OpenGL's z -> -z
//Row Vector

//Our Code:
//https://learnopengl-cn.readthedocs.io/zh/latest/01%20Getting%20started/08%20Coordinate%20Systems/
//OpenGL RightHand Corodinate
//Column Vector

/*
             y
             |  /
             | /
 ------------|/-----------> x
            /|
           / |
          z
 */

#include <iostream>
using namespace std;

class Vector2D
{
    friend ostream & operator << (ostream &out, const Vector2D &v);
    
public:
	double x, y;
    
public:
	explicit Vector2D(void);
    explicit Vector2D(double v);
	explicit Vector2D(double x, double y);

public:
	double LengthSquare() const;
	double Length() const;
	void Normalize();
    
	double Dot(const Vector2D &vector) const;

	Vector2D operator-() const;
	Vector2D operator+(const Vector2D &vector) const;
    Vector2D operator*(const double scale) const;
};

ostream & operator << (ostream &out, const Vector2D &v);
