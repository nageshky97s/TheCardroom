#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>

class VertexBuffer
{
	private:
		unsigned int VB_ID;
	public:
		VertexBuffer()
		{

		}

		VertexBuffer(const void* data,unsigned int size )
		{
			glGenBuffers(1, &VB_ID);
			glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}
		void bind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
		}

		void unbind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		~VertexBuffer()
		{
			glDeleteBuffers(1, &VB_ID);
		}
		inline unsigned int GetID() const { return VB_ID; };

};