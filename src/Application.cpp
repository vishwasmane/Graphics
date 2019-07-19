#include<iostream>
#include <glew.h>
#include <GLFW/glfw3.h>
#include<fstream>
using namespace std;

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearErrors();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* functionName, const char* fileName, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] ("<< error <<") : "<<fileName <<", "<< functionName <<", "<<line<<std::endl;
		return false;
	}

	return true;
}

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

static unsigned int CompileShader(unsigned int type, const string& shaderSource)
{
	unsigned int id = glCreateShader(type);
	const char * src = shaderSource.c_str();
	glShaderSource(id, 1, &src, nullptr);

	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char*)alloca(length * sizeof(char));
		cout << "Failed to compile shader " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << endl;
		cout << message << endl;

		return 0;
	}

	return id;
}

static int CreateShader(const string& vertexShaderSource, const string& fragmentShaderSource)
{
	unsigned int shaderProgram = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	return shaderProgram;
}


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	 
	float positions[6] =
	{
		0.0, 0.0,
		1.0, 0.0,
		0.0, 1.0
	};

	unsigned int indices[3] = 
	{
		0, 1, 2
	};
	
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// For Modern OpenGL
	GLenum glewInitRes = glewInit();
	if (glewInitRes != GLEW_OK)
		std::cout << "GLEW initialization failed.." << std::endl;

	std::cout << glGetString(GL_VERSION)<< std::endl;

	unsigned int vertexBufferId;
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 6, positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	//Very important - tell OpenGL Layout of the buffer.
	// Here specifying about the position attribute of vertex. If we have multiple attributes we will have to write multiple glVertexAttribPointer()
	// for each attribute specificatio.
	//index - index position of attribute in buffer layout, size - count of fields like here we have 2 floats for each vertex "position" attribute
	//type - datatype, isNormalized - 0-1, stride - size of bytes to reach to next memory location of attribute-"position here".
	//pointer - start position offset of the attributes in bytes. E.g suppose we have one more attribute for each vertex.(x, y, textureCoord). So prt will be 8 = <x,y = 2*floats>.
	// But parameter is const void* so need conversion like -> (const void*)8.
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	unsigned int indexBufferId;
	glGenBuffers(1, &indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3, indices, GL_STATIC_DRAW);

	// Shader
	const char* vertexShaderSourceFile = "VertexShader.txt";
	const char* fragmentShaderSourceFile = "FragmentShader.txt";
	string vertexShaderCodeString = ReadShaderCode(vertexShaderSourceFile);
	string fragmentShaderCodeString = ReadShaderCode(fragmentShaderSourceFile);
	int shaderProgramID = CreateShader(vertexShaderCodeString.c_str(), fragmentShaderCodeString.c_str());

	glUseProgram(shaderProgramID);

	float r = 0.0f;
	//float increment = 0.05f;

	GLint uColorLocation = glGetUniformLocation(shaderProgramID, "u_Color");
	glUniform4f(uColorLocation, r, 0.0f, 0.0f, 1.0f);
	bool increment = true;
	float step = 0.001f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgramID);
		// Legacy OpenGL
		/*glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_TRIANGLES);
		glVertex2d(0.0, 0.0);
		glVertex2d(1.0, 0.0);
		glVertex2d(0.0, 1.0);
		glEnd();*/
		printf("r value = %f \n", r);
		
		if (increment  && r < 1.0f)
		{
			r += step;
			if(r >= 1.0f)
				increment = false;
		}
		else if (!increment && r >= 0.0f)
		{
			r -= step;
			if (r <= 0)
				increment = false;
		}
		else if(r < 0.0f)
		{
			increment = true;
			r += step;
		}
		else if (r > 1.0f)
		{
			increment = false;
			r -= step;
		}
		
		
		glUniform4f(uColorLocation, r, 0.0f, 0.0f, 1.0f);
		

		// Modern OpenGL
		//glDrawArrays(GL_TRIANGLES, 0, 6); // No of indices that is no of vertices for drawing triangles.
		GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr));
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}