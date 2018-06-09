#pragma once

#include "hitable.h"

class shpere : public hitable
{
public:
	shpere()
	{

	}

	shpere(vec3 cen, float r)
		:center(cen), radius(r)
	{

	}

	virtual bool hit(const ray & r, float t_min, float t_max, hit_record &rec) const;

private:
	vec3 center;
	float radius;
};

bool shpere::hit(const ray & r, float t_min, float t_max, hit_record &rec) const
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0f * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float  discriminant = b*b - 4*a*c;
	if (discriminant > 0)
	{
		float temp = (-b - sqrt(discriminant)) / (2.0f * a);
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.pointer_at_parameter(temp);
			rec.normal = (rec.p - center) / radius;
			return true;
		}

		temp = (-b + sqrt(discriminant)) / (2.0f * a);
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.pointer_at_parameter(temp);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
	}
	return false;
}