#pragma once
#include<string>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "FreetypeRender.h"
#include "TextureImage.h"
#include "Global_objects.h"

float screen_height=600.0;
float screen_width = 800.0;
float resize_factor[] = { 1,1 };


class Button {

	

public:
	
	static double  c_xpos, c_ypos;
	 
	std::string text_on_button;
	bool but_status;
	glm::vec3 color;
	glm::vec2 position;
	bool hover;
	float outline_positions[4];
	float scale;
	
	Button()
	{
		text_on_button = "";
		but_status = true;
		hover = false;
		

	}


	void Init(const char* text, float xposition, float yposition, FreetypeRender& f,float scal) {
		text_on_button = text;
		but_status = true;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		position = glm::vec2(xposition, yposition);
		scale = scal;
		hover = false;
		f.obtain_boundary(text_on_button,position.x,position.y,scale,outline_positions);
		
		
	}
	void highlight_on_hover(FreetypeRender& f)
	{
		

		if (c_xpos > outline_positions[0]*resize_factor[0] && c_xpos < outline_positions[1] * resize_factor[0] &&
			c_ypos > outline_positions[3] * resize_factor[1] && c_ypos < outline_positions[2] * resize_factor[1])
		
		{
			hover = true;
			
		}
		else
		{
			hover = false;
		}
	}

	

	int on_click()
	{
		
		SoundEngine->play2D(click_sound, false);
		clicked = false;
		if (but_status)
			return 0;
		return -1;
			
	}

	void hide_button()
	{
		//do not render and on_click() shouldn't work 
		but_status = false;
	}

	void activate_button()
	{
		but_status = true;
	}

	
	
	int render_button(Shader &shader_text, FreetypeRender &f) // Store the pointer to the shader in freetype so that you dont have to pass that		
	{
		this->highlight_on_hover(f);

		if (hover)
			color =glm::vec3(1.0f, 0.0f, 0.0f);
		else
			color =glm::vec3(1.0f, 1.0f, 1.0f);


		 if(but_status) {
			
			f.RenderText(shader_text, text_on_button, position.x, position.y, scale,color);
		}

		if (clicked&& hover && but_status)
		{
			return this->on_click();
		}
		return -1;
	}


	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		//std::cout <<xpos << " " << ypos << std::endl;
		cursor_xpos = c_xpos = xpos;
		cursor_ypos = c_ypos = ypos;
		 
	}


	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{


		//if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			clicked = true;
		}
		else {
			clicked = false;
		}
	}


	
	~Button()
	{

	}


};