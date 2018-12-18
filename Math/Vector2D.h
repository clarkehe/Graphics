#pragma once

//https://learnopengl-cn.readthedocs.io/zh/latest/01%20Getting%20started/08%20Coordinate%20Systems/
//OpenGL RightHand Corodinate
//Column Vector

#include <iostream>
using namespace std;

class Vector2D
{
    friend ostream & operator << (ostream &out, const Vector2D &v);
    
public:
	double x, y;
    
public:
	Vector2D(void);
	Vector2D(double x, double y);

public:
	double LengthSquare();
	double Length();

	void Normalize();
	double Dot(const Vector2D &vector);

	Vector2D operator-();
	Vector2D operator+(const Vector2D &vector);
	Vector2D operator*(const double scale);
};

ostream & operator << (ostream &out, const Vector2D &v);

