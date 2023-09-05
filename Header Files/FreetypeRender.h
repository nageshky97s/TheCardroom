#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<ft2build.h>
#include FT_FREETYPE_H
#include <unordered_map>
#include <string>
#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<Shader.h>
#include<VertexBuffer.h>
#include<IndexBuffer.h>

class FreetypeRender
{
	private:
		FT_Library ft;
		FT_Face face;
		struct Character {
			unsigned int TextureID;
			glm::ivec2 Size;
			glm::ivec2 Bearing;
			unsigned int Advance;
		};
		unsigned int VAO, VBO;
		std::unordered_map<char, Character> Characters;
	
	public:
		
		FreetypeRender()
		{
			
		}

		FreetypeRender(const char* fonttype, unsigned int height, unsigned int width)
		{
			if (FT_Init_FreeType(&ft))
			{
				std::cout << "Error::FreeType : Could not intialize the FreeType Library" << std::endl;
			}

			if (FT_New_Face(ft, fonttype, 0, &face))
			{
				std::cout << "Error::FreeType: Failed to load font" << std::endl;
			}
			//defining Pixel size of the font
			FT_Set_Pixel_Sizes(face, width, height);


			glGenVertexArrays(1, &VAO);

			
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			

		}

		void load()
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			for (unsigned char c = 0; c < 128; c++)
			{
				
				//load character glyph 
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					std::cout << "Error::Freetype: Failed to load Glyph" << std::endl;
				}
				unsigned int texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
				};
				Characters.insert(std::pair<char, Character>(c, character));
			
			}
			FT_Done_Face(face);
			FT_Done_FreeType(ft);
		}

		void RenderText(Shader &s, std::string text, float x,float y, float scale, glm::vec3 color )// have to abstract VAO and make changes here
		{
			
			
			s.use();
			s.setFloatVec3("textColor",color);
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(VAO);

			//iterate thorugh all the characters
			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); c++)
			{
				Character ch = Characters[*c];

				float xpos = x + ch.Bearing.x * scale;
				float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

				float w = ch.Size.x * scale;
				float h = ch.Size.y * scale;
				
				//Update the VBO for each character
				float vertices[6][4] = {
					{ xpos,     ypos + h,   0.0f, 0.0f },
					{ xpos,     ypos,       0.0f, 1.0f },
					{ xpos + w, ypos,       1.0f, 1.0f },

				   { xpos,     ypos + h,   0.0f, 0.0f },
				   { xpos + w, ypos,       1.0f, 1.0f },
				   { xpos + w, ypos + h,   1.0f, 0.0f }
				};

								

				glBindTexture(GL_TEXTURE_2D, ch.TextureID);
				// update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				// render quad
				glDrawArrays(GL_TRIANGLES, 0, 6);
				
				// now advance cursors for next glyph
				x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
			}
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			
		}

		void obtain_boundary(std::string text, float x, float y, float scale,float* out_pos)
		{	
			out_pos[0] = out_pos[2] = 100000.0f;
			out_pos[1] = out_pos[3] = -100000.0f;
			
			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); c++)
			{
				Character ch = Characters[*c];

				float xpos = x + ch.Bearing.x * scale;
				float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

				float w = ch.Size.x * scale;
				float h = ch.Size.y * scale;

				if (out_pos[0] > xpos)
					out_pos[0] = xpos;
				if (out_pos[1] < xpos + w)
					out_pos[1] = xpos + w;
				if (out_pos[2] > ypos)
					out_pos[2] = ypos;
				if (out_pos[3] < ypos + h)
					out_pos[3] = ypos + h;



				x += (ch.Advance >> 6) * scale;

			}
			
			out_pos[2] = 600 - out_pos[2];
			out_pos[3] = 600 - out_pos[3];
			
		}
		
		
};
