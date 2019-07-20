#include<iostream>
#include <glew.h>
#include <GLFW/glfw3.h>
#include<fstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

using namespace std;

int main(void)
{
	GLFWwindow* window;

	glewExperimental = GL_TRUE;
 
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	//Create window in s[ecfied OpenGL context/version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);//GLFW_OPENGL_COMPATIBLE_PROFILE as it default creates/initiates VertexArrayObjects. 

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

	GLCall(std::cout << glGetString(GL_VERSION)<< std::endl);

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	VertexBuffer vb(positions, 3 * 2 * sizeof(GL_FLOAT));

	//Very important - tell OpenGL Layout of the buffer.
	// Here specifying about the position attribute of vertex. If we have multiple attributes we will have to write multiple glVertexAttribPointer()
	// for each attribute specificatio.
	//index - index position of attribute in buffer layout, size - count of fields like here we have 2 floats for each vertex "position" attribute
	//type - datatype, isNormalized - 0-1, stride - size of bytes to reach to next memory location of attribute-"position here".
	//pointer - start position offset of the attributes in bytes. E.g suppose we have one more attribute for each vertex.(x, y, textureCoord). So prt will be 8 = <x,y = 2*floats>.
	// But parameter is const void* so need conversion like -> (const void*)8.
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0)); //Gives error at this line as for core profile it doesn't have default 

	//Note : IF we want to add other vertexbuffer/object/mest to the current vao, we can do like below
	//Obviously we need other vertex buffer as well, which can have 3d or differrent layout which wde specify below
	//GLCall(glEnableVertexAttribArray(1));
	//GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0)); //Gives error at this line as for core profile it doesn't have default 

	IndexBuffer ib(indices, 3);

	// Shader
	const char* vertexShaderSourceFile = "shaders\\VertexShader.txt";
	const char* fragmentShaderSourceFile = "shaders\\FragmentShader.txt";
	Shader shader(vertexShaderSourceFile, fragmentShaderSourceFile);

	float r = 0.0f;
	GLint uColorLocation = shader.GetUniformLocation("u_Color");
	GLCall(glUniform4f(uColorLocation, r, 0.0f, 0.0f, 1.0f));
	bool increment = true;
	float step = 0.001f;

	//Disable everything
	GLCall(glBindVertexArray(0));
	shader.UnBind();
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	glDisableVertexAttribArray(0);
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		// Compute color to use for drawing - color animation
		if (increment  && r < 1.0f)
		{
			r += step;
			if (r >= 1.0f)
				increment = false;
		}
		else if (!increment && r >= 0.0f)
		{
			r -= step;
			if (r <= 0)
				increment = false;
		}
		else if (r < 0.0f)
		{
			increment = true;
			r += step;
		}
		else if (r > 1.0f)
		{
			increment = false;
			r -= step;
		}
		
		//This is kind of used for rendering multiple objects with differrent data and shaders.
		shader.Bind();
		GLCall(glUniform4f(uColorLocation, r, 0.0f, 0.0f, 1.0f));

		GLCall(glBindVertexArray(vao));
		ib.Bind();
		
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