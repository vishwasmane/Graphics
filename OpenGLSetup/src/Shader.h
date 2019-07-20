#pragma once
#include "Renderer.h"

class Shader
{
private:
	unsigned int m_Renderer;

public:
	Shader(const char * vertexShaderSourceFile, const char * fragmentShaderSourceFile);
	~Shader();

	void Bind();
	void UnBind();
	GLuint GetUniformLocation(const char * uniformVariableName);

};
