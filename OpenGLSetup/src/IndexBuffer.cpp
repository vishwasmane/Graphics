#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(unsigned int * indices, unsigned int size)
{

	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &m_Renderer));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), indices, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_Renderer));
}

void IndexBuffer::Bind()
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer));
}

void IndexBuffer::UnBind()
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
