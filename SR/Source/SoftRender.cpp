#include "stdafx.h"
#include "SoftRender.h"
#include "Render.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace SoftRender;

#ifndef M_PI_2
    #define M_PI_2     1.57079632679489661923   // pi/2
#endif

GLuint loadFarmeBufferToTexture(Color *frameBufer, int width, int height);

uint testRender()
{
	const int WIDTH = 1024, HEIGHT = 768;
	Render render(WIDTH, HEIGHT);

	render.SetFrustum(M_PI_2, float(WIDTH)/(float)HEIGHT, 0.1f, 1000.0f);
	render.SetCamera(Vec3f(0, 0, 15.0f));

	Light light(
		Vec3f(0.0f, 0.0f, 100.0f),
		Color(0.2f, 0.2f, 0.2f),
		Color(0.5f, 0.5f, 0.5f),
		Color(1.0f, 1.0f, 1.0f)
	);
	render.SetLight(light);

	/*Material m(
		Color(1.0f, 0.5f, 0.31f),
		Color(1.0f, 0.5f, 0.31f),
		Color(0.5f, 0.5f, 0.5f),
		32.0f);
	*/
    
	//render.currentMode = Textured;
	//Model cube(std::string("../../../SR/Res/nanosuit/nanosuit.obj"), Vec3f(-10, -5, -5), m);
	//render.DrawModel(cube);

	render.currentMode = Textured;
	Model nanosuit2(std::string("../../../SR/Res/crystal_maiden/crystal_maiden_econ.fbx"),
                    Vec3f(10, -75, -150));
	render.DrawModel(nanosuit2);

	SaveTexture(render.frameBuffer, WIDTH, HEIGHT, "screenshot.jpg");
	return loadFarmeBufferToTexture(render.frameBuffer, WIDTH, HEIGHT);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

GLuint loadFarmeBufferToTexture(Color *frameBufer, int width, int height)
{
	void *data = frameBufer;

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, data);

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}

