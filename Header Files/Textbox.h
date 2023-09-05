#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"Shader.h"
#include "TextureImage.h"
#include "FreetypeRender.h"
#include "Global_objects.h"
#include <string>
class Textbox {

public:
	TextureImage bckgrd;
	TextureImage bckgrd_clicked;
	std::string value;
	bool clicked_inside_box;
	Textbox(std::string path1, std::string path2,int tex_no1, int tex_no2,float x,float y,float z,float scale_x, float scale_y, float scale_z) {
		bckgrd = TextureImage(path1, tex_no1);
		bckgrd_clicked = TextureImage(path2, tex_no2);

		this->x = x;
		this->y = y;
		this->z = z;
		this->scale_x = scale_x;
		this->scale_y = scale_y;
		this->scale_z = scale_z;
		value = "";
		clicked = false;
	}
	void render(Shader& s,FreetypeRender& f, Shader& t) {
		read_input_stream();
		s.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(x, y, z)); //300.0f, -215.0f, 0.0f
		model = glm::scale(model, glm::vec3(scale_x, scale_y, scale_z));
		s.setMatrix4fv("model", model);
		if (clicked_inside_box)
		{
			bckgrd_clicked.bind();
			bckgrd_clicked.render(s);
		}
		else {
			bckgrd.bind();
			bckgrd.render(s);
		}
		
		
		//f.RenderText(t, value, x + 5.0f, y, 0.2f, glm::vec3(0.5f, 0.5f, 0.5f));

		f.RenderText(t, value, x*2+15.0f, 75.f, 0.25f, glm::vec3(0.0f, 0.0f, 0.0f)); // generalize this
	}
	void read_input_stream() {
		if (clicked && cursor_xpos >= 610.0f*resize_factor[0] && cursor_xpos <= 790.0f * resize_factor[0] &&
			cursor_ypos >= 500.0f * resize_factor[1] && cursor_ypos <= 530.0f * resize_factor[1])//610 790 500 530
		{
			clicked_inside_box = true;
			keyboard_stream = value;
		}
		else if (clicked)
		{
			clicked_inside_box = false;
			
		}
		if (clicked_inside_box)
		{
			value = keyboard_stream;		
				
		}
		
	}

	std::string textbox_value()
	{
		std::cout << value.empty() << std::endl;
		return value;
	}

	void clear_textbox()
	{
		value = "";
		keyboard_stream = "";

	}
private:
	
	float x; 
	float y;
	float z;
	float scale_x;
	float scale_y;
	float scale_z;
};


