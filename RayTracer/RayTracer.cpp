// RayTracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "vec3.h"
#include "ray.h"
#include <assert.h>

//http://paulcuth.me.uk/netpbm-viewer/

void CreatePPM()
{
	int nx = 200;
	int ny = 100;
	std::cout <<  "P3\n" << nx << " " << ny << "\n255\n";

	for (int j=ny-1; j>=0; j--)
	{
		for (int i=0; i<nx; i++)
		{
			float r = float(i) / nx;
			float g = float(j) / ny;
			float b = 0.2f;

			int ir = int (255.99*r);
			int ig = int (255.99*g);
			int ib = int (255.99*b);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}

void CreatePPM2()
{
	int nx = 200;
	int ny = 100;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";

	for (int j=ny-1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(float(i)/nx, float(j)/ny, 0.2f);

			int ir = int (255.99 * col[0]);
			int ig = int (255.99 * col[1]);
			int ib = int (255.99 * col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}

vec3 color(const ray & r)
{
	vec3 unit_direction = unit_vector(r.direction()); //[-1, 1]
	float t = 0.5f * (unit_direction.y() + 1);		  //[0, 1]
	return (1.0f -  t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f); //lerp
}

void CreatePPM3()
{
	int nx = 200;
	int ny = 100;
	std::cout <<  "P3\n" << nx << " " << ny << "\n255\n";

	vec3 lower_left_colr(-2.0f, -1.0f, -1.0f);
	vec3 horizontal(4.0f, 0, 0);
	vec3 vertical(0, 2.0f, 0);
	vec3 origin(0, 0, 0);

	for (int j=ny-1; j>=0; j--)
	{
		for (int i=0; i < nx; i++)
		{
			float u = float(i) / nx;
			float v = float(j) / ny;

			ray r(origin, lower_left_colr + u * horizontal + v * vertical);
			vec3 col = color(r);

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}

bool hit_sphere(const vec3 &center, float radius, const ray &r)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0f * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float  discriminant = b*b - 4*a*c;
	return discriminant > 0;
}

vec3 color4(const ray &r)
{
	if (hit_sphere(vec3(0, 0, -1), 0.5, r))
	{
		return vec3(1, 0, 0);
	}
	return color(r);
}

void CreatePPM4()
{
	int nx = 200;
	int ny = 100;
	std::cout <<  "P3\n" << nx << " " << ny << "\n255\n";

	vec3 lower_left_colr(-2.0f, -1.0f, -1.0f);
	vec3 horizontal(4.0f, 0, 0);
	vec3 vertical(0, 2.0f, 0);
	vec3 origin(0, 0, 0);

	for (int j=ny-1; j>=0; j--)
	{
		for (int i=0; i < nx; i++)
		{
			float u = float(i) / nx;
			float v = float(j) / ny;

			ray r(origin, lower_left_colr + u * horizontal + v * vertical);
			vec3 col = color4(r);

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}

float hit_sphere5(const vec3 &center, float radius, const ray &r)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0f * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float  discriminant = b*b - 4*a*c;
	if (discriminant < 0)
	{
		return -1;
	}
	else
	{
		float ret = (-b - sqrt(discriminant)) / (2.0f * a);
		assert(ret > 0);
		return ret;
	}
}

vec3 color5(const ray &r)
{
	float t = hit_sphere5(vec3(0, 0, -1), 0.5, r);
	if (t > 0)
	{
		vec3 N = unit_vector((r.pointer_at_parameter(t) - vec3(0, 0, -1)));
		return 0.5f * vec3(N.x()+1, N.y()+1, N.z()+1);
	}
	return color(r);
}

void CreatePPM5()
{
	int nx = 200;
	int ny = 100;
	std::cout <<  "P3\n" << nx << " " << ny << "\n255\n";

	vec3 lower_left_colr(-2.0f, -1.0f, -1.0f);
	vec3 horizontal(4.0f, 0, 0);
	vec3 vertical(0, 2.0f, 0);
	vec3 origin(0, 0, 0);

	for (int j=ny-1; j>=0; j--)
	{
		for (int i=0; i < nx; i++)
		{
			float u = float(i) / nx;
			float v = float(j) / ny;

			ray r(origin, lower_left_colr + u * horizontal + v * vertical);
			vec3 col = color5(r);

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}


#include "hitable.h"
#include "hitable_list.h"
#include "sphere.h"

vec3 color6(const ray &r, hitable *world)
{
	hit_record rec;
	if (world->hit(r, 0, 10000, rec))
	{
		return 0.5 * vec3(rec.normal.x() + 1, 
			rec.normal.y() + 1, 
			rec.normal.z() + 1);
	}
	else
	{
		return color(r);
	}
}

void CreatePPM6()
{
	int nx = 200;
	int ny = 100;
	std::cout <<  "P3\n" << nx << " " << ny << "\n255\n";

	vec3 lower_left_colr(-2.0f, -1.0f, -1.0f);
	vec3 horizontal(4.0f, 0, 0);
	vec3 vertical(0, 2.0f, 0);
	vec3 origin(0, 0, 0);

	hitable *list[2];
	list[0] = new shpere(vec3(0, 0, -1), 0.5f);
	list[1] = new shpere(vec3(0, -100.5f, -1), 100);
	hitable *world = new hitable_list(list, 2);

	for (int j=ny-1; j>=0; j--)
	{
		for (int i=0; i < nx; i++)
		{
			float u = float(i) / nx;
			float v = float(j) / ny;

			ray r(origin, lower_left_colr + u * horizontal + v * vertical);
			vec3 col = color6(r, world);

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}

//Antialiasing
#include "camera.h"

#define m 0x100000000LL  
#define c 0xB16  
#define a 0x5DEECE66DLL  

static unsigned long long seed = 1;  

//return [0, 1]
double drand48(void)  
{  
	seed = (a * seed + c) & 0xFFFFFFFFFFFFLL;  
	unsigned int x = (unsigned int)(seed >> 16);  
	return  ((double)x / (double)m);  
}  

void srand48(unsigned int i)  
{  
	seed  = (((long long int)i) << 16) | rand();  
}  

void CreatePPM7()
{
	int nx = 200;
	int ny = 100;
	int ns = 100;

	std::cout <<  "P3\n" << nx << " " << ny << "\n255\n";

	hitable *list[2];
	list[0] = new shpere(vec3(0, 0, -1), 0.5f);
	list[1] = new shpere(vec3(0, -100.5f, -1), 100);
	hitable *world = new hitable_list(list, 2);

	camera cam;
	for (int j=ny-1; j>=0; j--)
	{
		for (int i=0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + drand48()) / nx;
				float v = float(j + drand48()) / ny;

				//每个点，有ns条ray，前面只有一条
				ray r = cam.get_ray(u, v);
				col += color6(r, world);
			}

			col /= float(ns);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}

vec3 random_in_unit_shpere()
{
	vec3 p;
	do 
	{
		p = 2.0f * vec3(drand48(), drand48(), drand48()) - vec3(1.0f, 1.0f, 1.0f);
	} while (p.squared_lenght() >= 1);
	return p;
}

vec3 color8(const ray &r, hitable *world)
{
	hit_record rec;
	if (world->hit(r, 0, 100000, rec))
	{
		//根据射线与world的交点，随机构造反射射线，与world再次进行hit检测，直到与World与没交点。
		vec3 target = rec.p + rec.normal + random_in_unit_shpere();
		return 0.5f * color8(ray(rec.p, target - rec.p), world);
	}
	else
	{
		//direction 的y值。
		return color(r);
	}
}

void CreatePPM8()
{
	int nx = 200;
	int ny = 100;
	int ns = 100;

	std::cout <<  "P3\n" << nx << " " << ny << "\n255\n";

	hitable *list[2];
	list[0] = new shpere(vec3(0, 0, -1), 0.5f);
	list[1] = new shpere(vec3(0, -100.5f, -1), 100);
	hitable *world = new hitable_list(list, 2);

	camera cam;
	for (int j=ny-1; j>=0; j--)
	{
		for (int i=0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + drand48()) / nx;
				float v = float(j + drand48()) / ny;

				//每个点，有ns条ray，前面只有一条
				ray r = cam.get_ray(u, v);
				col += color8(r, world);
			}

			col /= float(ns);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}

int main(int argc, char* argv[])
{
	CreatePPM8();
	return 0;
}
