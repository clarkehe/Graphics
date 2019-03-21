#include "stdafx.h"
#include "SoftRender.h"
#include "Render.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define HALF_PI  1.57079632679489661923   // pi/2

GLuint loadFarmeBufferToTexture(Color *frameBufer, int width, int height);

uint testRender()
{
	const int WIDTH = 1024, HEIGHT = 768;
	Render render(WIDTH, HEIGHT);

	render.SetFrustum(HALF_PI, float(WIDTH)/(float)HEIGHT, 0.1f, 1000.0f);
	render.SetCameraPos(Vec3f(0, 0, 15.0f));

    Light light;
    light.pos = Vec3f(0, 0, 10);
    light.color = Color(1, 1, 1, 1);
    
	render.SetLight(light);
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
