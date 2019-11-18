/* This is a utility program that helps OpenGL programmers check their program for errors. 
The following functions are provided.

// Print the content of an aiScene object. 
// Call this function after Assimp::Importer.ReadFile(). 
void printAiSceneInfo(const aiScene* scene, AiScenePrintOption option); 

// Call this function after glCompileShader().
void printShaderInfoLog(GLuint shaderID)

// Call this function after glLinkProgram() or glValidateProgram().
void printShaderProgramInfoLog(GLuint shaderProgID)

// Print OpenGL error message, if any. 
// Call this function at the end of a function that contains OpenGL APIs. 
void checkOpenGLError()

// OpenGL Debug context callback function
// openGLDebugCallback()

Written by Ying Zhu
Department of Computer Science
Georgia State University

2014

*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//------------------------------------------------------------
// Use this function to check the return values of OpenGL APIs
// such as glCreateShader(), glCreateProgram(), glCreateShaderProgram(),
// and glCreateShaderProgramv().  
void checkGlCreateXError(int variable, string name) {
	if(variable == 0) {
		cout << "\nThere is an error creating " << name << endl;
	}	
}

//------------------------------------------------------------
// Use this function to check the return values of OpenGL APIs
// glGetAttribLocation() and glGetUniformLocation().
void checkGlGetXLocationError(int variableLoc, string name) {
	if(variableLoc < 0) {
		cout << "\nThere is an error getting the location of GLSL variable " << name << endl;
		cout << "This error may occur if " << name 
		<< " does not exist, or if " << name << " is not used in the shader program." << endl;
	}
}


//-----------------------------------------------------------
// Print out the output of the shader compiler. 
// Call this function after glCompileShader(). 
// If there is no error, OpenGL doesn't generate any message. 
void printShaderInfoLog(GLuint shaderID)
{
	unsigned int maxLength = 1024;
    char infoLog[1024];
	 int infologLength = 0;
 
	if (glIsShader(shaderID)) {
		glGetShaderInfoLog(shaderID, maxLength, &infologLength, infoLog);
	} else {
		cout << "printShaderInfoLog(): Invalid shader ID" << shaderID << endl;
	}
 
	if (infologLength > 0) {
		cout << "Shader: " << infoLog << endl;
	}
}

//--------------------------------------------
// Print out the output of the shader program.
// Call this function after glLinkProgram() or glValidateProgram().
// If there is no error, OpenGL doesn't generate any message. 
void printShaderProgramInfoLog(GLuint shaderProgID)
{
	unsigned int maxLength = 1024;
    char infoLog[1024];
	 int infologLength = 0;
 
	if (glIsProgram(shaderProgID)) {
		glGetProgramInfoLog(shaderProgID, maxLength, &infologLength, infoLog);
	} else {
		cout << "printShaderProgramInfoLog(): Invalid shader program ID" << shaderProgID << endl;
	}
 
	if (infologLength > 0) {
		cout << "Shader program: " << infoLog;
	}
}

//------------------------------------
// Convert an error type to a printable character string. 
const char* genErrorString( GLenum error )
{
    const char*  msg = NULL;

	// A clever technique to convert an OpenGL type to a character string.
	// The following are all the error types returned by glGetError(). 
    switch( error ) {
#define Case( Token )  case Token: msg = #Token; break;
	Case( GL_NO_ERROR );
	Case( GL_INVALID_VALUE );
	Case( GL_INVALID_ENUM );
	Case( GL_INVALID_OPERATION );
	Case( GL_STACK_OVERFLOW );
	Case( GL_STACK_UNDERFLOW );
	Case( GL_OUT_OF_MEMORY );
	Case( GL_INVALID_FRAMEBUFFER_OPERATION);
#undef Case	
    }

    return msg;
}

//----------------------------------------------------------------------------
// Print OpenGL error message, if any. 
void checkOpenGLError(string name = "")
{
    GLenum  error = glGetError();

	//If more than one flag has recorded an error, glGetError returns 
	// and clears an arbitrary error flag value. Thus, glGetError should always 
	// be called in a loop, until it returns GL_NO_ERROR.
    while(error != GL_NO_ERROR) {
		const char* msg = genErrorString(error);
		if (msg != NULL) {
			cout << "OpenGL error";
			
			if (name.length() > 0) {
				cout << " in " << name;  
			}

			cout << ": " << msg << endl;
		}

		// Continue to get error messsage until there is no error
		error = glGetError();
    } 
}

// This callback function displayes the debug message sent by OpenGL.
// Register this function by calling
// glDebugMessageCallback(openGLDebugCallback, nullptr);
void  openGLDebugCallback(GLenum source, GLenum type, GLuint id, 
			  GLenum severity, GLsizei length, 
			  const GLchar* message, void* userParam) {
    cout << "---------- OpenGL Debug Message ----------" << endl;
    cout << "message: "<< message << endl;
    cout << "type: ";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        cout << "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        cout << "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        cout << "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        cout << "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        cout << "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        cout << "OTHER";
        break;
    }
    cout << endl;
 
    cout << "id: " << id << endl;
	
	cout << "severity: ";
    switch (severity){
    case GL_DEBUG_SEVERITY_LOW:
        cout << "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        cout << "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        cout << "HIGH";
        break;
    }
    cout << endl;
    cout << "---------- OpenGL Debug Message End ----------" << endl;
}

//-----------------------------------------------------------------
// Initialize OpenGL context and OpenGL Debug Context if necessary. 
void initOpenGLDebugContext(bool enableDebugContext) {
		GLint majorVersion = 0;
		GLint minorVersion = 0;

		// Get the major and minor versions of OpenGL on this computer.
		glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

		cout << "This computer supports OpenGL version " << majorVersion << "." << minorVersion << endl;

		// Must explicity specify OpenGL context 4.3 for the debug callback function to work. 
		//glutInitContextVersion(majorVersion, minorVersion);
    
		// Debug context is only supported since OpenGL 4.3.
		if (enableDebugContext && (majorVersion >= 4) && (minorVersion >= 3)) {
			// Initialize OpenGL debug context, which is available from OpenGL 4.3+. 
			//glutInitContextFlags(GLUT_DEBUG);

			// Usually you want to enable GL_DEBUG_OUTPUT_SYNCHRONOUS so that error messages are 
			// immediately reported. 
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

			// Register a debug message callback function.
			// The callback function is defined in the check_error.hpp file.  
			glDebugMessageCallback((GLDEBUGPROC)openGLDebugCallback, nullptr);

			cout << "OpenGL Debug Context enabled." << endl;
		}
		else {
			cout << "OpenGL Debug Context is disabled." << endl;
		}

	checkOpenGLError("initOpenGLContext"); // error checking
}
