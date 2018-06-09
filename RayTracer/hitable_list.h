#pragma once

#include "hitable.h"

class hitable_list : public hitable
{
public:
	hitable_list()
	{
		list = NULL;
		list_size = 0;
	}

	hitable_list(hitable **l, int n)
		:list(l), list_size(n)
	{

	}

	virtual bool hit(const ray & r, float t_min, float t_max, hit_record &rec) const;

private:
	hitable **list;
	int list_size;
};

bool hitable_list::hit(const ray & r, float t_min, float t_max, hit_record &rec) const
{
	hit_record tem_rec;
	bool hit_anything = false;
	float closest_so_far = t_max;

	for (int i=0; i<list_size; i++)
	{
		if (list[i]->hit(r, t_min, closest_so_far, tem_rec))
		{
			hit_anything = true;
			closest_so_far = tem_rec.t;
			rec = tem_rec;
		}
	}

	return hit_anything;
}
