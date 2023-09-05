  #pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include "TextureImage.h"
#include "FreetypeRender.h"
#include "Button.h"
#include "Global_objects.h"
#include "HandRanking.h"


class MainScreen {

public:
	GLFWwindow* window;
	Shader shaderBasic;
	Shader shaderText;
	Button home_Play_Button;
	TextureImage tex_home_back;
	//unsigned int VAO;
	FreetypeRender Main_Screen_Text;
	//VertexBuffer verBuf;
	//IndexBuffer indBuf;
	

	MainScreen()
	{
		
		//Setting up of GLFW 
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		window = glfwCreateWindow(800, 600, "The Cardroom", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to Create Window" << std::endl;
			glfwTerminate();
			//return -1;
		}
		glfwMakeContextCurrent(window);


		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//Passing GLAD the to load the address of the OpenGL function pointers which is OS-specific
		{
			std::cout << "Failed to intialize GLAD" << std::endl;
			//return -1;

		}

		
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		//glDepthMask(GL_FALSE);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, Button::cursor_position_callback);
		glfwSetMouseButtonCallback(window, Button::mouse_button_callback);
		glfwSetKeyCallback(window, key_callback);
		

	}

	void setup()  {

		
		

		shaderBasic.Init("D:/C_Proj/The Cardroom/res/ShaderPrograms/shader.vs.txt", "D:/C_Proj/The Cardroom/res/ShaderPrograms/shader.fs.txt");
		shaderText.Init("D:/C_Proj/The Cardroom/res/ShaderPrograms/freetypeshader.vs.txt", "D:/C_Proj/The Cardroom/res/ShaderPrograms/freetypeshader.fs.txt");
		

		

		//verBuf = VertexBuffer(vertices, sizeof(vertices));
		//indBuf = IndexBuffer(indices, 6);//six refer to the no of unsigned int, multiplication is happening in the IndexBuffer class

		tex_home_back= TextureImage("D:/C_Proj/The Cardroom/res/Textures/Pokemon.jpg", 1);
		tex_home_back.bind();//binds to slot zero if no no is mentioned
		



		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(400, 300,1));

		glm::mat4 view = glm::mat4(1.0f);
		//glm::mat4 projection = glm::ortho(-800.0f / 2.0f, 800.0f / 2.0f, -600.0f / 2.0f, 600.0f / 2.0f,-1.0f,1.0f);
		glm::mat4 projection = glm::ortho(-800.0f / 2.0f, 800.0f / 2.0f, -600.0f / 2.0f, 600.0f / 2.0f,-1.0f,1.0f);

	

		shaderBasic.use();
		shaderBasic.setMatrix4fv("projection", projection);
		shaderBasic.setMatrix4fv("view", view);
		shaderBasic.setMatrix4fv("model", model);
		shaderBasic.setInt("texture1", 0);
		

		//projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f,0.1f,0.2f);
		projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f ,-1.0f,1.0f);
		shaderText.use();
		shaderText.setMatrix4fv("projection", projection);



		Main_Screen_Text= FreetypeRender("D:/C_Proj/The Cardroom/res/Fonts/Antonio-Bold.ttf", 0, 96);
		Main_Screen_Text.load();
	

		// Button creation ( text,button_xpos,button_ypos,border-yes/no)

		home_Play_Button.Init("Play CPU", 520.0f, 530.0f, Main_Screen_Text, 0.5f);

		
	}

	int run() {

		
		tex_home_back.render(shaderBasic);
		
		Main_Screen_Text.RenderText(shaderText, "The Cardroom", 25.0f, 25.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
		return home_Play_Button.render_button(shaderText, Main_Screen_Text);


	}

	void destroy()
	{

		shaderBasic.deleteShader();
		shaderText.deleteShader();
		//verBuf.unbind();
		//indBuf.unbind();


	}




	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		resize_factor[0] = width / screen_width;
		resize_factor[1] = height/screen_height;
		std::cout << "Window resized" << std::endl;
		std::cout << resize_factor[0] <<"  " << resize_factor[1]<< std::endl;
		std::cout <<"New Dimensions:"<< width<< "  " << height<< std::endl;
		std::cout <<"Screen Dimensions:"<< screen_width<< "  " << screen_height<< std::endl;

	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key== GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			
			glfwSetWindowShouldClose(window, true);
		}
		else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
		{
			if (keyboard_stream.size() < 8)
				keyboard_stream += "0";
		}
		else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		{
			if (keyboard_stream.size() < 8)
				keyboard_stream += "1";
		}
		else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		{
			if (keyboard_stream.size() < 8)
				keyboard_stream += "2";
		}
		else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		{
			if (keyboard_stream.size() < 8)
				keyboard_stream += "3";
		}
		else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
		{
			if (keyboard_stream.size() < 8)
				keyboard_stream += "4";
		}
		else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
		{
			if (keyboard_stream.size() < 8)
				keyboard_stream += "5";
		}
		else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
		{
			if (keyboard_stream.size() < 8)
				keyboard_stream += "6";
		}
		else if (key == GLFW_KEY_7 && action == GLFW_PRESS)
		{
			if (keyboard_stream.size() < 8)
				keyboard_stream += "7";
		}
		else if (key == GLFW_KEY_8 && action == GLFW_PRESS)
		{
			if (keyboard_stream.size() < 8)
				keyboard_stream += "8";
		}
		else if (key == GLFW_KEY_9 && action == GLFW_PRESS)
		{
			if (keyboard_stream.size() < 8)
				keyboard_stream += "9";
		}
		else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)//backspace key 
		//else if (key == GLFW_KEY_BACKSPACE )//backspace key 
		{
			if(keyboard_stream.size()>0)
				keyboard_stream.pop_back();
		}
		



	}


};