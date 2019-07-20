#pragma once

class IndexBuffer
{
private:
	unsigned int m_Renderer;

public:
	IndexBuffer(unsigned int * indices, unsigned int size);
	~IndexBuffer();

	void Bind();
	void UnBind();
};
