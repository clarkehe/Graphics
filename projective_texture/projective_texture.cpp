/*
   This example demonstrates projective texture mapping, which
   is the basis for shadow mapping, lightmap, and texture decal.
  
   In regular texture mapping, the texture coordinates from each
   vertex is static, often calculated in the modeling software
   such as Maya, 3DS Max, or Blender. In projective texture mapping,
   the texture coordinates for each vertex are not static,
   but calculated from the position of the vertice. The basic idea
   is to project the vertex from the projector's viewpoint to a
   2D picture. The 2D coordinates of the projected vertex are then
   used as its texture coordinates.
  
   First, construct a model-view-projection-bias matrix
   from the projector's perspective, and then pass this projectorMVPB
   matrix to the vertex shader. In the vertex shader, multiply
   the vertex position with this projectorMVPB matrix, and then use
   the first two component's of the transformed position as
   the texture coordinates for this vertex.

   The key function is buildProjectorMatrices(), which builds the
   model-view-projection-bias matrix for the projector. Then in the
   vertex shader, the following line of code calculates the
   texture coordinates for the vertex:
       textureCoord = (textureMatrix * vPos).xy;

 
Ying Zhu
Georgia State University

November 2016
*/

// https://sites.google.com/site/gsucomputergraphics/educational/advanced-texture-mapping/projective-texture-mapping

// GLEW header
#include <GL/glew.h> // This must appear before freeglut.h

// Include GLFW
#include <GLFW/glfw3.h>

// Freeglut header
//#include <GL/freeglut.h>
#include <GLUT/GLUT.h>

// GLM header files
#include <glm/glm.hpp> 

#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp> 

// Simple OpenGL Image Library header file
#include "SOIL/SOIL.h"

#include "check_error.hpp"

// C++ header files
#include <iostream>

using namespace std;
using namespace glm;

#define BUFFER_OFFSET(offset) ((GLvoid *) offset)

/*
// A square with two triangles.
GLfloat vertices[][4] = {
	{ 1.368670f, 1.321428f, 0.094483f, 1.0f },
	{ -1.274187f, 1.321429f, 0.094483f, 1.0f },
	{ -1.274187f, -1.321428f, 0.094483f, 1.0f },
	{ 1.368670f, 1.321428f, 0.094483f, 1.0f },
	{ -1.274187f, -1.321428f, 0.094483f, 1.0f },
	{ 1.368670f, -1.321429f, 0.094483f, 1.0f }
};

int numVertices = 6;

GLfloat normals[][4] = {
	{ 0.000000f, 0.000000f, 1.000000f, 1.0f },
	{ 0.000000f, 0.000000f, 1.000000f, 1.0f },
	{ 0.000000f, 0.000000f, 1.000000f, 1.0f },
	{ 0.000000f, 0.000000f, 1.000000f, 1.0f },
	{ 0.000000f, 0.000000f, 1.000000f, 1.0f },
	{ 0.000000f, 0.000000f, 1.000000f, 1.0f },
};

// Specify texture coordinates.
GLfloat textureCoords[][2] = {
	{ 0.000000f, 0.000000f },
	{ 1.000000f, 0.000000f },
	{ 1.000000f, 1.000000f },
	{ 0.000000f, 0.000000f },
	{ 1.000000f, 1.000000f },
	{ 0.000000f, 1.000000f }
};

*/

// A pyramid object.
GLfloat vertices[][4] = {
	{ 1.0f, -1.0f, 1.0f, 1.0f }, // face 1
	{ -1.0f, -1.0f, -1.0f, 1.0f },
	{ 1.0f, -1.0f, -1.0f, 1.0f },
	{ 1.0f, -1.0f, -1.0f, 1.0f }, // face 2
	{ 0.0f, 1.0f, 0.0f, 1.0f },
	{ 1.0f, -1.0f, 1.0f, 1.0f },
	{ 1.0f, -1.0f, 1.0f, 1.0f }, // face 3
	{ 0.0f, 1.0f, 0.0f, 1.0f },
	{ -1.0f, -1.0f, 1.0f, 1.0f },
	{ -1.0f, -1.0f, 1.0f, 1.0f }, // face 4
	{ 0.0f, 1.0f, 0.0f, 1.0f },
	{ -1.0f, -1.0f, -1.0f, 1.0f },
	{ 0.0f, 1.0f, 0.0f, 1.0f }, // face 5
	{ 1.0f, -1.0f, -1.0f, 1.0f },
	{ -1.0f, -1.0f, -1.0f, 1.0f },
	{ 1.0f, -1.0f, 1.0f, 1.0f }, // face 6
	{ -1.0f, -1.0f, 1.0f, 1.0f },
	{ -1.0f, -1.0f, -1.0f, 1.0f }
};

int numVertices = 18;

GLfloat normals[][4] = {
	{ 0.0f, -1.0f, 0.0f, 1.0f }, // normal 1
	{ 0.0f, -1.0f, 0.0f, 1.0f },
	{ 0.0f, -1.0f, 0.0f, 1.0f },
	{ 0.8944f, 0.4472f, 0.0f, 1.0f }, // normal 2
	{ 0.8944f, 0.4472f, 0.0f, 1.0f },
	{ 0.8944f, 0.4472f, 0.0f, 1.0f },
	{ -0.0f, 0.4472f, 0.8944f, 1.0f }, // normal 3
	{ -0.0f, 0.4472f, 0.8944f, 1.0f },
	{ -0.0f, 0.4472f, 0.8944f, 1.0f },
	{ -0.8944f, 0.4472f, 0.0f, 1.0f }, // normal 4
	{ -0.8944f, 0.4472f, 0.0f, 1.0f },
	{ -0.8944f, 0.4472f, 0.0f, 1.0f },
	{ 0.0f, 0.4472f, -0.8944f, 1.0f }, // normal 5
	{ 0.0f, 0.4472f, -0.8944f, 1.0f },
	{ 0.0f, 0.4472f, -0.8944f, 1.0f },
	{ 0.0f, -1.0f, 0.0f, 1.0f }, // normal 6
	{ 0.0f, -1.0f, 0.0f, 1.0f },
	{ 0.0f, -1.0f, 0.0f, 1.0f }
};

GLfloat textureCoords[][2] = {
	{ 0.0319, 0.4192 }, // textureCoord 1
	{ 0.3546, 0.0966 },
	{ 0.3546, 0.4192 },
	{ 0.4223, 0.5177 },	// textureCoord 2
	{ 0.2541, 0.8753 },
	{ 0.0996, 0.5116 },
	{ 0.8047, 0.5250 },	// textureCoord 3
	{ 0.6434, 0.8857 },
	{ 0.4820, 0.5250 },
	{ 0.6637, 0.0981 },	// textureCoord 4
	{ 0.5130, 0.4184 },
	{ 0.3748, 0.0926 },
	{ 0.8416, 0.4227 },	// textureCoord 5
	{ 0.6922, 0.0988 },
	{ 0.9834, 0.0954 },
	{ 0.0319, 0.4192 },	// textureCoord 6
	{ 0.0319, 0.0966 },
	{ 0.3546, 0.0966 }
};

// Remeber to update the texture image file path for your computer. 
const char *imageFileName = "../../../projective_texture/marble1.jpg";
const char *projectiveTextureImageName = "../../../projective_texture/mona_lisa.jpg";

// VBO buffer IDs
GLuint vertexArrayBufferID = 0;
GLuint normalArrayBufferID = 0;
GLuint texCoordArrayBufferID = 0;

GLuint program; // shader program ID

// Shader variable IDs
GLint vPos; // vertex attribute: position
GLint normalID; // vertex attribute: normal
GLint textureCoordID; // vertex attribute: texture coordinates

GLint mvpMatrixID; // uniform variable: model, view, projection matrix
GLint modelMatrixID; // uniform variable: model, view matrix
GLint normalMatrixID; // uniform variable: normal matrix for transforming normals
GLint textureMatrixID; // uniform variable: texture matrix for calculating texture 
                       // coordinates for projective texture mapping

GLint lightSourcePositionID; // uniform variable: for lighting calculation
GLint diffuseLightProductID; // uniform variable: for lighting calculation
GLint ambientID;
GLint attenuationAID;
GLint attenuationBID;
GLint attenuationCID;

GLint textureSamplerID; // texture sampler ID
GLint projTextureSamplerID; // projective texture sampler ID

// Texture object ID
GLuint textureID;
GLuint projTextureID;

// Texture unit ID
// These two values must be consistent. 
GLenum textureUnitID = GL_TEXTURE1;
// Sometimes, setting this to 0 will connect the sampler to any active texture unit. 
// But it's better to avoid using this "trick". 
GLuint textureSamplerValue = 1;

// Texture unit ID for the projective texture
// These two values must be consistent. 
GLenum projTextureUnitID = GL_TEXTURE2;
// Sometimes, setting this to 0 will connect the sampler to any active texture unit. 
// But it's better to avoid using this "trick". 
GLuint projTextureSamplerValue = 2;

// Transformation matrices
mat4 projMatrix;
mat4 mvpMatrix;
mat4 modelMatrix;
mat4 viewMatrix;
mat3 normalMatrix;  // Normal matrix for transforming normals

mat4 textureCoordTransformationMatrix;

// Light parameters
vec4 lightSourcePosition = vec4(0.0f, 4.0f, 0.0f, 1.0f);
vec4 diffuseMaterial = vec4(0.5f, 0.5f, 0.5f, 1.0f);
vec4 diffuseLightIntensity = vec4(1.0f, 1.0f, 1.0f, 1.0f);
vec4 ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
float attenuationA = 1.0f;
float attenuationB = 0.2f;
float attenuationC = 0.0f;

vec4 diffuseLightProduct;

// Camera parameters
vec3 eyePosition = vec3(0.0f, 0.0f, 4.0f);
vec3 lookAtCenter = vec3(0.0f, 0.0f, 0.0f);
vec3 upVector = vec3(0.0f, 1.0f, 0.0f);
float fieldOfView = 30.0f;
float nearPlane = 0.1f;
float farPlane = 1000.0f;

// Mouse controlled rotation angles
float rotateX = 45;
float rotateY = 0;

// Projector parameters
// Define the projector's location and orientation. 
vec3 projectorPosition = vec3(0.0f, 1.0f, 0.0f);
vec3 projectorLookAtPosition = vec3(0.5f, 0.0f, 0.0f);
vec3 projectorUpVector = vec3(0.0f, 0.0f, 1.0f);
float projectorFOV = 60.0f;

//---------------------------------------------------------------
// Initialize vertex arrays and VBOs
void prepareVBOs() {
	// Define a 3D pyramid. 

	// Get an unused buffer object name. Required after OpenGL 3.1. 
	glGenBuffers(1, &vertexArrayBufferID);

	// If it's the first time the buffer object name is used, create that buffer. 
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBufferID);

	// Allocate memory for the active buffer object. 
	// 1. Allocate memory on the graphics card for the amount specified by the 2nd parameter.
	// 2. Copy the data referenced by the third parameter (a pointer) from the main memory to the 
	//    memory on the graphics card. 
	// 3. If you want to dynamically load the data, then set the third parameter to be NULL. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &normalArrayBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, normalArrayBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	// Create a buffer object to store the texture coordinates. 
	// Later we will passt his over to the shader. 
	glGenBuffers(1, &texCoordArrayBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordArrayBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
}

//-------------------------------------------------------------------
void prepareShaders() {
	// Vertex shader source code
	// The vertex position and normal vector are transformed and then passed on to the fragment shader. 
	const char* vSource = {
		"#version 330 core\n"
		"in vec4 vPos;"
		"in vec4 normal;"
		"in vec2 vTextureCoord;"

		"uniform mat4x4 mvpMatrix;"
		"uniform mat4x4 modelMatrix;"
		"uniform mat3x3 normalMatrix;"

		// The model-view-projection matrix for the projector
		// This matrix is used to calculate the texture coordinates for the vertex. 
		"uniform mat4x4 textureMatrix;"

		"out vec4 transformedPosition;"
		"out vec3 transformedNormal;"
		"out vec2 textureCoord;"
		"out vec4 textureCoordProj;"

		"void main() {"
		"	gl_Position = mvpMatrix * vPos;"
		// Transform the vertex position to the world space. 
		"	transformedPosition = modelMatrix * vPos;"
		// Transform the normal vector to the world space. 
		"   transformedNormal = normalize(normalMatrix * normal.xyz);"
		
		"   textureCoord = vTextureCoord;"

		// Calculates the texture coordinates for the vertex
		// Project the vertex to a 2D picture and then take the 2D coordinates as 
		// the texture coordinates. 
		"   textureCoordProj = textureMatrix * modelMatrix * vPos;"

		// If modelMatrix is removed from this calculation, the projected texture will
		// "stick" to the object, rather than be fixed in space. This is called texture decal. 
		// "   textureCoordProj = textureMatrix * vPos;"
		"}"
	};

	// Fragment shader source code
	// A point light source is implemented. 
	// For simplicity, only the ambient and diffuse components are implemented. 
	// The lighting is calculated in world space, not in camera space. 

	// Note that the transformedPosition and transformedNormal in the fragment shader are for each fragment (pixel).
	// They are interpolated from the vertex positions and vertex normals in the (invisible) rasterization stage. 
	const char* fSource = {
		"#version 330 core\n"
		"in vec4 transformedPosition;"
		"in vec3 transformedNormal;"
		"in vec2 textureCoord;"
		"in vec4 textureCoordProj;"

		"uniform vec4 lightSourcePosition;"
		"uniform vec4 diffuseLightProduct;"
		"uniform vec4 ambient;"
		"uniform float attenuationA;"
		"uniform float attenuationB;"
		"uniform float attenuationC;"

		// Don't use the reserved word "texture" as variable name. 
		"uniform sampler2D tex;"  // Regular texture image 
		"uniform sampler2D texProj;"  // Projected texture image

		"out vec4 fragColor;"
		"void main() {"
		// Light direction
		"	vec3 lightVector = normalize(transformedPosition.xyz - lightSourcePosition.xyz);"
		// Distance between the light source and vertex
		"   float dist = distance(lightSourcePosition.xyz, transformedPosition.xyz);"
		// Attenuation factor
		"   float attenuation = 1.0f / (attenuationA + (attenuationB * dist) + (attenuationC * dist * dist));"
		// Calculate the diffuse component of the lighting equation.
		"	vec4 diffuse = attenuation * (max(dot(transformedNormal, lightVector), 0.0) * diffuseLightProduct);"

		// For regular texture mapping
		"   vec4 textureColor = texture(tex, textureCoord);"

		// For perspective projection effect
		"   vec4 textureColorProj = textureProj(texProj, textureCoordProj);"

		// Combine the ambient component and diffuse component. 
		"	vec4 lightTextureMix = mix((ambient + diffuse), textureColor, 0.6f);"

		"   fragColor = mix(lightTextureMix, textureColorProj, 0.2f);"
		"}"
	};

	// Declare shader IDs
	GLuint vShader, fShader;

	// Create empty shader objects
	vShader = glCreateShader(GL_VERTEX_SHADER);
	checkGlCreateXError(vShader, "vShader");

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	checkGlCreateXError(fShader, "fShader");

	// Attach shader source code the shader objects
	glShaderSource(vShader, 1, &vSource, NULL);
	glShaderSource(fShader, 1, &fSource, NULL);

	// Compile shader objects
	glCompileShader(vShader);
	printShaderInfoLog(vShader);

	glCompileShader(fShader);
	printShaderInfoLog(fShader);

	// Create an empty shader program object
	program = glCreateProgram();
	checkGlCreateXError(program, "program");

	// Attach vertex and fragment shaders to the shader program
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	// Link the shader program
	glLinkProgram(program);
	printShaderProgramInfoLog(program);
}

//---------------------------------------------------------------
// Retrieve the IDs of the shader variables. Later we will
// use these IDs to pass data to the shaders. 
void getShaderVariableLocations(GLuint shaderProgram) {

	// Retrieve the ID of a vertex attribute, i.e. position
	vPos = glGetAttribLocation(shaderProgram, "vPos");
	checkGlGetXLocationError(vPos, "vPos");

	normalID = glGetAttribLocation(shaderProgram, "normal");
	checkGlGetXLocationError(normalID, "normal");

	// get the ID of the texture coordinate variable in the shader. 
	textureCoordID = glGetAttribLocation(shaderProgram, "vTextureCoord");
	checkGlGetXLocationError(textureCoordID, "vTextureCoord");

	mvpMatrixID = glGetUniformLocation(shaderProgram, "mvpMatrix");
	checkGlGetXLocationError(mvpMatrixID, "mvpMatrix");

	modelMatrixID = glGetUniformLocation(shaderProgram, "modelMatrix");
	checkGlGetXLocationError(modelMatrixID, "modelMatrix");

	normalMatrixID = glGetUniformLocation(shaderProgram, "normalMatrix");
	checkGlGetXLocationError(normalMatrixID, "normalMatrix");

	lightSourcePositionID = glGetUniformLocation(shaderProgram, "lightSourcePosition");
	checkGlGetXLocationError(lightSourcePositionID, "lightSourcePosition");

	diffuseLightProductID = glGetUniformLocation(shaderProgram, "diffuseLightProduct");
	checkGlGetXLocationError(diffuseLightProductID, "diffuseLightProduct");

	ambientID = glGetUniformLocation(shaderProgram, "ambient");
	checkGlGetXLocationError(ambientID, "ambient");

	attenuationAID = glGetUniformLocation(shaderProgram, "attenuationA");
	checkGlGetXLocationError(attenuationAID, "attenuationA");

	attenuationBID = glGetUniformLocation(shaderProgram, "attenuationB");
	checkGlGetXLocationError(attenuationBID, "attenuationB");

	attenuationCID = glGetUniformLocation(shaderProgram, "attenuationC");
	checkGlGetXLocationError(attenuationCID, "attenuationC");

	// get the ID of the texture sampler in the shader. 
	textureSamplerID = glGetUniformLocation(shaderProgram, "tex");
	checkGlGetXLocationError(textureSamplerID, "tex");

	textureMatrixID = glGetUniformLocation(shaderProgram, "textureMatrix");
	checkGlGetXLocationError(textureMatrixID, "textureMatrix");

	projTextureSamplerID = glGetUniformLocation(shaderProgram, "texProj");
	checkGlGetXLocationError(projTextureSamplerID, "texProj");
}

//---------------------------------------------------------------
void setShaderVariables() {
	// value_ptr is a glm function
	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, value_ptr(mvpMatrix));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, value_ptr(modelMatrix));
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, value_ptr(normalMatrix));

	glUniformMatrix4fv(textureMatrixID, 1, GL_FALSE, value_ptr(textureCoordTransformationMatrix));

	glUniform4fv(lightSourcePositionID, 1, value_ptr(lightSourcePosition));
	glUniform4fv(diffuseLightProductID, 1, value_ptr(diffuseLightProduct));
	glUniform4fv(ambientID, 1, value_ptr(ambient));
	glUniform1f(attenuationAID, attenuationA);
	glUniform1f(attenuationBID, attenuationB);
	glUniform1f(attenuationCID, attenuationC);

	// Specify the current active texture unit number. 
	//
	// There are certain number of texture units on the graphics card (for example 32).
	// Check OpenGL extension viewer to find out how many texture units are on your card. 
	//
	// The shader accesses the texture through texture units, not the texture IDs. 
	// In other words, the shader does not know the texture IDs; it only sees 
	// (for example) 32 texture units.
	// You can have more than 32 texture images stored in the graphics card's memory, but
	// at any given time, the shader only sees 32 texture units. In the OpenGL program,
	// you can associate a texture unit with any texture ID, and change it at any time. 
	// This is how you can transfer potentially large numbers of texture images to the 
	// shader through a limited channel. 
	// 
	glActiveTexture(textureUnitID);

	// Bind the texture ID with the active texture unit number. 
	// In this case, texture ID "textureID" is associated with texture unit #2. 
	// When the shader reads texture unit #2, it will read the texture identified by ID textureID. 
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Associate texture sampler ID in the shader with the active texture unit number.
	// glActiveTexture() function and glUniform1i(textUnit, ...) function calls must be consistent. 
	// If you change texture unit number in one function all, you must change the texture unit 
	// in the other function call. 
	glUniform1i(textureSamplerID, textureSamplerValue);

	glActiveTexture(projTextureUnitID);
	glBindTexture(GL_TEXTURE_2D, projTextureID);

	glUniform1i(projTextureSamplerID, projTextureSamplerValue);
}

//---------------------------------------------------------------
// Set lighting related parameters
void setLightingParam() {
	diffuseLightProduct = diffuseMaterial * diffuseLightIntensity;
}

void prepareTextureImage() {
    
    char *test = (char *)glGetString( GL_EXTENSIONS );
    
	// Use SOIL to load a picture. 
	textureID = SOIL_load_OGL_texture(imageFileName,
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y); // Adding SOIL_FLAG_MIPMAPS is also fine. 

	// SOIL will call glGenTexture() and glTexImage2D() to specify textures. 
	// If textureID is 0, then there is an error. 
	checkGlCreateXError(textureID, "textureID");

	projTextureID = SOIL_load_OGL_texture(projectiveTextureImageName,
		SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS); // Adding SOIL_FLAG_MIPMAPS is also fine. 

	// SOIL will call glGenTexture() and glTexImage2D() to specify textures. 
	// If textureID is 0, then there is an error. 
	checkGlCreateXError(projTextureID, "projTextureID");
}

//---------------------------------------------------------------
// Build the model matrix. This matrix will transform the 3D object to the proper place. 
mat4 buildModelMatrix() {

	mat4 rotationXMatrix = rotate(mat4(1.0f), radians(rotateX), vec3(1.0f, 0.0f, 0.0f));
	mat4 rotationYMatrix = rotate(mat4(1.0f), radians(rotateY), vec3(0.0f, 1.0f, 0.0f));

	mat4 matrix = rotationYMatrix * rotationXMatrix;

	return matrix;
}

//---------------------------------------------------------------
// This function build a view-projection matrix from a
// projector's viewpoint. 
mat4 buildProjectorMatrices() {

	// Construct a view-projection matrix as if the camera is placed at the projector.
	// If we use this view-projection matrix to transform the object, the object will
	// be projected from the projector to a 2D space, which is the texture space
	// for the projector view. The texture coordinates in this 2D space are the
	// texture coordinates for the projective texture. 

	// The view matrix from the projector's viewpoint. 
	mat4 projectorViewMatrix = lookAt(projectorPosition,
		projectorLookAtPosition, projectorUpVector);

	// The projection matrix for the projector. 
	mat4 projectorProjectionMatrix = perspective(projectorFOV, 1.0f, 0.5f, 10.0f);
	// mat4 projectorProjectionMatrix = ortho(0.3f, 0.3f, 0.3f, 0.0f, 0.5f, 10.0f);

	// After the initial projection, the origin is at the center of the window. 
	// However, the origin of a texture image is at the lower left corner. 
	// The scale and bias matrix is used to transform the origin from the center 
	// to the corner. 
	mat4 scaleBiasMatrix = scale(translate(mat4(1.0f), vec3(0.5, 0.5, 0.5)), vec3(0.5f));

	// Construct the combined matrix to transform the vertex position to its projective
	// texture coordiantes. 
	mat4 projectorTransformMatrix = scaleBiasMatrix * projectorProjectionMatrix *
		projectorViewMatrix;

	return projectorTransformMatrix;
}

//---------------------------------------------------------------
void buildMatrices() {
	modelMatrix = buildModelMatrix();

	mvpMatrix = projMatrix * viewMatrix * modelMatrix;

	normalMatrix = column(normalMatrix, 0, vec3(modelMatrix[0][0], modelMatrix[0][1], modelMatrix[0][2]));
	normalMatrix = column(normalMatrix, 1, vec3(modelMatrix[1][0], modelMatrix[1][1], modelMatrix[1][2]));
	normalMatrix = column(normalMatrix, 2, vec3(modelMatrix[2][0], modelMatrix[2][1], modelMatrix[2][2]));

	// Use glm::inverseTranspose() to create a normal matrix, which is used to transform normal vectors. 
	normalMatrix = inverseTranspose(normalMatrix);

	textureCoordTransformationMatrix = buildProjectorMatrices();
}

//---------------------------------------------------------------
// Handles the display event
void display()
{
	// Clear the window with the background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	buildMatrices();

	setShaderVariables();

	// Activate the shader program
	glUseProgram(program);

	// If the buffer object already exists, make that buffer the current active one. 
	// If the buffer object name is 0, disable buffer objects. 
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBufferID);

	// Associate the vertex array in the buffer object with the vertex attribute: "position"
	glVertexAttribPointer(vPos, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// Enable the vertex attribute: "position"
	glEnableVertexAttribArray(vPos);

	glBindBuffer(GL_ARRAY_BUFFER, normalArrayBufferID);
	glVertexAttribPointer(normalID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(normalID);

	// Associate the texture coordinate array in the buffer object with the vertex attribute "vTextureCoord", 
	// which is identified by texCoordID. 
	glBindBuffer(GL_ARRAY_BUFFER, texCoordArrayBufferID);
	glVertexAttribPointer(textureCoordID, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(textureCoordID);

	// Start the shader program. Draw the object. The third parameter is the number of vertices. 
	glDrawArrays(GL_TRIANGLES, 0, numVertices);

	// Refresh the window
	//glutSwapBuffers();

	//checkOpenGLError();
}

//---------------------------------------------------------------
// Handles the reshape event
void reshape(int width, int height)
{
	// Specify the width and height of the picture within the window
	glViewport(0, 0, width, height);

	projMatrix = perspective(fieldOfView, (float)width / (float)height, nearPlane, farPlane);

	viewMatrix = lookAt(eyePosition, lookAtCenter, upVector);
}

//---------------------------------------------------------------
// Read mouse motion data and convert them to rotation angles. 
void passiveMotion(int x, int y) {

	rotateY = (float)x * -0.5f;
	rotateX = (float)y * 0.5f;

	// Generate a dislay event to force refreshing the window. 
	glutPostRedisplay();
}

//-----------------------------------------------------------------
void init() {
	prepareVBOs();

	prepareShaders();

	getShaderVariableLocations(program);

	setLightingParam();

	prepareTextureImage();

	// Specify the background color
	glClearColor(1, 1, 1, 1);

	glEnable(GL_DEPTH_TEST);

	// Most of the OpenGL API calls are made from this function.
	// So we need to check error here. 
	checkOpenGLError();
}

//---------------------------------------------------------------
int main(int argc, char *argv[])
{
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
    
    // Open a window and create its OpenGL context
    GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow( 1024, 768, "Projective Texture", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window); // Initialize GLEW
    
    // We would expect width and height to be 1024 and 768
    int windowWidth = 1024;
    int windowHeight = 768;
    // But on MacOS X with a retina screen it'll be 1024*2 and 768*2, so we get the acual framebuffer size:
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    
    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    init();
    
    //VA0
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    do{
        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear( GL_COLOR_BUFFER_BIT );
        
        reshape(windowWidth, windowHeight);
        display();
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    return 0;
}
