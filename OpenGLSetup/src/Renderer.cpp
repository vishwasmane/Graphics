#include "Renderer.h"
#include <iostream>

void GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* functionName, const char* fileName, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ") : " << fileName << ", " << functionName << ", " << line << std::endl;
		return false;
	}

	return true;
}