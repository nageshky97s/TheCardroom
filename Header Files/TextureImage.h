#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include<iostream>
#include "Shader.h"
#include"stb_image.h"


class TextureImage {

	private:
		unsigned int textureID;
		std::string FilePath;
		unsigned char* localBuffer;
		int width, height, BPP;
		unsigned int bind_num;
		unsigned int VAO;
		unsigned int VBO, EBO;

	public:

		TextureImage()
		{
		}

		TextureImage(const std::string& path,unsigned int bn)
			:textureID(0), FilePath(path), localBuffer(nullptr), width(0), height(0), BPP(0), bind_num(0)
		{
			float vertices[] = {
				// positions          // texture coords
				 1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
				 1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
				-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
				-1.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left 
			};


			unsigned int indices[] = {
				 0,1,3,
				 1,2,3
			};

			
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			
			// texture coord attribute
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);


			bind_num = bn;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);

			stbi_set_flip_vertically_on_load(1);
			//float borderColor[] = { 1.0f,1.0f,1.0f,1.0f };
			//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			localBuffer = stbi_load(FilePath.c_str(), &width, &height, &BPP,0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE );
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST );
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			
			if (localBuffer)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, localBuffer);
				glGenerateMipmap(GL_TEXTURE_2D);
				stbi_image_free(localBuffer);
			}
			else {
				std::cout<< "Failed to load the texture"<<std::endl;
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			
		}
		

		void bind() const
		{
			
			//glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureID);

		}
		void unbind() const
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		inline int getWidth() const { return width; };
		inline int getHeight() const { return height; };
		inline int getBindNum() const { return bind_num; };

		void render(Shader &s) const
		{
			this->bind();
			s.use(); 
			glBindVertexArray(VAO);		
			
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			//glDrawArrays(GL_TRIANGLES, 0, 3);
			
		}
		


};