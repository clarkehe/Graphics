#include "test.h"
#include "Render.h"
#include "Material.h"

using namespace SoftRender;
using namespace std;

test::test(void)
{
}

test::~test(void)
{
}

void test::testMatrix()
{
	Mat4f a;
	a[0][0] = 3.567f;
	a[0][1] = 4.535f;
	a[0][2] = 5.9808f;
	a[2][1] = 3.0f;
	a[3][2] = 5.36985f;
	a[3][1] = 7.9087f;
	a[1][2] = 5.78f;
}

#ifndef M_PI_2
    #define M_PI_2     1.57079632679489661923   // pi/2
#endif

void test::testRender()
{
	const int WIDTH = 1024, HEIGHT = 768;
	Render render(WIDTH, HEIGHT);

	render.SetFrustum(M_PI_2, float(WIDTH)/(float)HEIGHT, 0.1f, 1000.0f);
	render.SetCamera(Vec3f(0, 0, 15.0f));

    Light light;
    light.worldPos = Vec3f(0.0f, 0.0f, 100.0f);
    light.ambient = Color(0.2f, 0.2f, 0.2f);
    light.diffuse = Color(0.5f, 0.5f, 0.5f);
    light.specular = Color(1.0f, 1.0f, 1.0f);
                           
	render.SetLight(light);
	render.currentMode = Textured;
                           
	Material m(
		Color(1.0f, 0.5f, 0.31f),
		Color(1.0f, 0.5f, 0.31f),
		Color(0.5f, 0.5f, 0.5f),
		32.0f);
	
	render.currentMode = WireFrame;
	Model cube(std::string("cube.obj"), Vec3f(0.0f, 0, -5), m);
	render.DrawModel(cube);

	SaveTexture(render.frameBuffer, WIDTH, HEIGHT, "screenshot.jpg");
}

