// Include standard headers
#include "stdafx.h"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <shader.hpp>
#include <texture.hpp>
#include <controls.hpp>
#include <objloader.hpp>
#include <vboindexer.hpp>

#include "SoftRender.h"

#ifdef _MSC_VER

LONGLONG CalculateFrequency()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return frequency.QuadPart;
}

///////////////////////////////////////////////////////////////////////////////////

// Updated once on boot
static const LONGLONG TIMER_FREQUENCY = CalculateFrequency(); 

struct TimeProfile
{
	TimeProfile()
	{
		LARGE_INTEGER largeInteger;
		QueryPerformanceCounter( &largeInteger );
		mStartTime =  largeInteger.QuadPart;
	}

	~TimeProfile()
	{
		LARGE_INTEGER largeInteger;
		QueryPerformanceCounter( &largeInteger );
		LONGLONG endTime = largeInteger.QuadPart;

		LONGLONG costTime = endTime - mStartTime;
		double time = costTime * 1000.0f;
		time /= TIMER_FREQUENCY;

		printf("cost Time: %f \n", time);
	}

	LONGLONG mStartTime;
};

#endif

#if 1

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    //The following two lines needed for 3.3 on Mac OSx
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = nullptr;
    
    // Open a window and create its OpenGL context
	if( !(window=glfwCreateWindow( 1024, 768,  "Test", nullptr, nullptr )) )
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
    
    glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
    
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window,  GLFW_STICKY_KEYS, GL_TRUE );
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
    
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

    //Very important!!!
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
    
	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "../../../SR/StandardShading.vertexshader",
                                    "../../../SR/StandardShading.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	/*
		2   3
		0   1
	*/

	//0 1 2
	vertices.push_back(glm::vec3(-1, -1, 0));
	vertices.push_back(glm::vec3(1, -1, 0));
	vertices.push_back(glm::vec3(-1, 1, 0));

	//1 3 2
	vertices.push_back(glm::vec3(1, -1, 0));
	vertices.push_back(glm::vec3(1, 1, 0));
	vertices.push_back(glm::vec3(-1, 1, 0));

	uvs.push_back(glm::vec2(0, 1-0));
	uvs.push_back(glm::vec2(1, 1-0));
	uvs.push_back(glm::vec2(0, 1-1));

	uvs.push_back(glm::vec2(1, 1-0));
	uvs.push_back(glm::vec2(1, 1-1));
	uvs.push_back(glm::vec2(0, 1-1));

	normals.push_back(glm::vec3(0, 0, 1));
	normals.push_back(glm::vec3(0, 0, 1));
	normals.push_back(glm::vec3(0, 0, 1));

	normals.push_back(glm::vec3(0, 0, 1));
	normals.push_back(glm::vec3(0, 0, 1));
	normals.push_back(glm::vec3(0, 0, 1));

	// Load it into a VBO
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	//GLuint mGLBuffer;
	//{
	//	glGenBuffers( 1, &mGLBuffer );
	//	glBindBuffer( GL_ARRAY_BUFFER, mGLBuffer );
	//	glBufferData( GL_ARRAY_BUFFER, 5*1024*1024, NULL, GL_STREAM_DRAW );
	//}

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    //
	const GLuint renderTex = testRender();

	do{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		//computeMatricesFromInputs(window);
        
        glm::mat4 ProjectionMatrix = glm::ortho(-4/3.0f, 4/3.0f, -1.0f, 1.0f, 0.1f, 100.0f);
        
        glm::vec3 position = glm::vec3( 0, 0, 1 );
        
        // Camera matrix
        glm::mat4 ViewMatrix       = glm::lookAt(
                                       position,           // Camera is here
                                       glm::vec3( 0, 0, 0 ),
                                       //position+direction, // and looks here : at the same position, plus "direction"
                                       glm::vec3( 0, 1, 0 )
                                       //up                  // Head is up (set to 0,-1,0 to look upside-down)
                                       );
        
		//glm::mat4 ProjectionMatrix = getProjectionMatrix();
		//glm::mat4 ViewMatrix = getViewMatrix();
        
		glm::mat4 ModelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(4/3.0f, 1.0f, 1.0f));
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

        //
		glm::vec3 lightPos = glm::vec3(4,4,4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderTex);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		//{
		//	//TimeProfile tem;
		//	glBindBuffer( GL_ARRAY_BUFFER, mGLBuffer );
		//	glMapBufferRange( GL_ARRAY_BUFFER, 0, 5*1024*1024, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
		//}

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0  );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &renderTex);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

#endif
