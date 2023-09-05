#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>


class IndexBuffer
{
private:
	unsigned int IB_ID;
	unsigned int IB_count;
public:

	IndexBuffer()
	{

	}

	IndexBuffer(const unsigned int* data, unsigned int count): IB_count(count)
	{		

		glGenBuffers(1, &IB_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW);
	}
	void bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB_ID);
	}

	void unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	~IndexBuffer()
	{
		glDeleteBuffers(1, &IB_ID);
	}

	inline unsigned int GetCount() const { return IB_count; };
	inline unsigned int GetID() const { return IB_ID; };
};