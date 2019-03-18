#include "Vector2D.hpp"
#include <math.h>

Vector2D::Vector2D(void)
{
	x = y = 0.f;
}

Vector2D::Vector2D(double v)
{
    this->x = this->y = v;
}

Vector2D::Vector2D(double x, double y)
{
	this->x = x;
	this->y = y;
}

double Vector2D::LengthSquare() const
{
	return x * x + y * y;
}

double Vector2D::Length() const
{
	return sqrt(LengthSquare());
}

void Vector2D::Normalize()
{
    const double len = Length();
    if (len <= std::numeric_limits<double>::epsilon())
        return;
    
    assert(len > 0);
    x /= len;
	y /= len;
}

double Vector2D::Dot(const Vector2D &vector) const
{
	return vector.x * x + vector.y * y;
}

Vector2D Vector2D::operator-() const
{
	return Vector2D(-x, -y);
}

Vector2D Vector2D::operator+(const Vector2D &vector) const
{
	return Vector2D(x + vector.x, y + vector.y);
}

Vector2D Vector2D::operator*(const double scale) const
{
	return Vector2D(x * scale, y * scale);
}

ostream & operator << (ostream &out, const Vector2D &v)
{
    out << v.x << "," << v.y << endl;
    return out;
}
