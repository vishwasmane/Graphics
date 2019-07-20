#include "Shader.h"
#include <fstream>
#include "Renderer.h"
#include <iostream>
using namespace std;

string ReadShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

unsigned int CompileShader(unsigned int type, const string& shaderSource)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char * src = shaderSource.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));

	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char *message = (char*)alloca(length * sizeof(char));
		cout << "Failed to compile shader " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << endl;
		cout << message << endl;

		return 0;
	}

	return id;
}

int CreateShader(const string& vertexShaderSource, const string& fragmentShaderSource)
{
	unsigned int shaderProgram = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	GLCall(glAttachShader(shaderProgram, vs));
	GLCall(glAttachShader(shaderProgram, fs));
	GLCall(glLinkProgram(shaderProgram));
	GLCall(glValidateProgram(shaderProgram));

	return shaderProgram;
}


Shader::Shader(const char * vertexShaderSourceFile, const char * fragmentShaderSourceFile)
{
	string vertexShaderCodeString = ReadShaderCode(vertexShaderSourceFile);
	string fragmentShaderCodeString = ReadShaderCode(fragmentShaderSourceFile);
	m_Renderer = CreateShader(vertexShaderCodeString.c_str(), fragmentShaderCodeString.c_str());

	GLCall(glUseProgram(m_Renderer))
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_Renderer))
}

void Shader::Bind()
{
	GLCall(glUseProgram(m_Renderer));
}

void Shader::UnBind()
{
	GLCall(glUseProgram(0));
}

GLuint Shader::GetUniformLocation(const char * uniformVariableName)
{
	GLCall(GLint uColorLocation = glGetUniformLocation(m_Renderer, uniformVariableName));
	return uColorLocation;
}
