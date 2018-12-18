#include "StdAfx.h"
#include "Vector2D.h"
#include <complex>

Vector2D::Vector2D(void)
{
	x = y = 0.f;
}

Vector2D::Vector2D(double x, double y)
{
	this->x = x;
	this->y = y;
}

double Vector2D::LengthSquare()
{
	return x * x + y * y;
}

double Vector2D::Length()
{
	return std::sqrt(LengthSquare());
}

void Vector2D::Normalize()
{
	x /= Length();
	y /= Length();
}

double Vector2D::Dot(const Vector2D &vector)
{
	return vector.x * x + vector.y * y;
}

Vector2D Vector2D::operator-()
{
	return Vector2D(-x, -y);
}

Vector2D Vector2D::operator+(const Vector2D &vector)
{
	return Vector2D(x + vector.x, y + vector.y);
}

Vector2D Vector2D::operator*(const double scale)
{
	return Vector2D(x * scale, y * scale);
}
