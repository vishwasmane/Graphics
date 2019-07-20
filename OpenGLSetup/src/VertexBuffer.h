#pragma once

class VertexBuffer
{
private:
	unsigned int m_Renderer;

public:
	VertexBuffer(float *vertexData, unsigned int size);
	~VertexBuffer();

	void Bind();
	void UnBind();
};