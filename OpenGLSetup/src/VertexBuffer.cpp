#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(float * vertexData, unsigned int size)
{
	GLCall(glGenBuffers(1, &m_Renderer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Renderer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertexData, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_Renderer));
}

void VertexBuffer::Bind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Renderer));
}

void VertexBuffer::UnBind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
