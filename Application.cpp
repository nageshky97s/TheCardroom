/*
Error Handling


*/
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include<irrKlang/irrKlang.h>
#include<thread>

#include"Shader.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include "TextureImage.h"
#include "FreetypeRender.h"
#include "Button.h"
#include "MainScreen.h"
#include "Model.h"
#include "Global_objects.h"
#include "Textbox.h"



#include "PokerEngineEvents.h"
#include "HandRanking.h"


double Button::c_xpos = 0.0;
double Button::c_ypos = 0.0;
//bool Button::clicked = false;
int clicked_value;
int state;
bool animate_state;
int gameOver_state;

int main()
{
	
	MainScreen mainscreen;

	//std::thread thr(&MainScreen::setup, mainscreen);
	mainscreen.setup();
	
	int context = -1;
	
	
	std::vector<std::tuple<unsigned int, unsigned int >> input_cards;
	int hr_state=-100;//hand_ranking input state
	
	Hand_and_strength place_hold;
	
	glm::mat4 projection = glm::ortho(-400.0 ,400.0, -300.0, 300.0,-300.0,300.0);	
	glm::mat4 view = glm::mat4(1.0f);	
	glm::mat4 model = glm::mat4(1.0f);	
	model = glm::translate(model, glm::vec3(-200.00f, 40.0f, 0.0f)); // translate it down so it's at the center of the scene	
	//model = glm::scale(model, glm::vec3(2.5f,2.5f,1.0f));	// it's a bit too big for our scene, so scale it down
	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene	
	model = glm::scale(model, glm::vec3(1.75f, 1.25f, 1.0f));	// it's a bit too big for our scene, so scale it down
	
	Shader Model_shader;
	Model_shader.Init("res/ShaderPrograms/Model.vs.txt", "res/ShaderPrograms/Model.fs.txt");
	Model_shader.use();	
	Model_shader.setMatrix4fv("projection", projection);	
	Model_shader.setMatrix4fv("view", view);
	Model_shader.setMatrix4fv("model", model);	
	Model Card_asset("res/Models/CardDeck2/CardDeck.FBX"); //2.5 scale works 400/300

	Shader imageShader;
	Shader textShader;
	TextureImage tex_play_back;
	TextureImage tex_play_back2;
	FreetypeRender Play_Screen_Text;

	imageShader.Init("res/ShaderPrograms/shader.vs.txt", "res/ShaderPrograms/shader.fs.txt");
	textShader.Init("res/ShaderPrograms/freetypeshader.vs.txt", "res/ShaderPrograms/freetypeshader.fs.txt");
	
	tex_play_back = TextureImage("res/Textures/CombinedImage_button1.jpg", 1);
	tex_play_back.bind();
	
	tex_play_back2 = TextureImage("res/Textures/CombinedImage_button2.jpg", 4);
	tex_play_back2.bind();


	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(400, 300, 1));

	view = glm::mat4(1.0f);
	//glm::mat4 projection = glm::ortho(-800.0f / 2.0f, 800.0f / 2.0f, -600.0f / 2.0f, 600.0f / 2.0f,-1.0f,1.0f);
	projection = glm::ortho(-800.0f / 2.0f, 800.0f / 2.0f, -600.0f / 2.0f, 600.0f / 2.0f, -1.0f, 1.0f);
	
	imageShader.use();
	imageShader.setMatrix4fv("projection", projection);
	imageShader.setMatrix4fv("view", view);
	imageShader.setMatrix4fv("model", model);
	imageShader.setInt("texture1", 0);

	projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);	
	textShader.use();
	textShader.setMatrix4fv("projection", projection);

	Play_Screen_Text = FreetypeRender("res/Fonts/Antonio-Bold.ttf", 0, 96);
	Play_Screen_Text.load();
	std::string ranknames[] = {"High Card","One Pair","Two Pair","Three of a kind","Straight","Flush" ,"Full House","Four of a Find","Straight Flush" };

	
	
	//Buttons- BACK, CALL, RAISE, CHECK, FOLD

	Button backButton;
	Button callButton;
	Button raiseButton;
	Button checkButton;
	Button foldButton;
	Button retryButton;

	backButton.Init("BACK", 15.0f, 575.0f, Play_Screen_Text, 0.25f);
	callButton.Init("CALL", 500.0f, 20.0f, Play_Screen_Text, 0.5f);
	raiseButton.Init("RAISE", 590.0f, 20.0f, Play_Screen_Text, 0.5f);
	checkButton.Init("CHECK", 470.0f, 20.0f, Play_Screen_Text, 0.5f);
	foldButton.Init("FOLD", 700.0f, 20.0f, Play_Screen_Text, 0.5f);
	retryButton.Init("RETRY?", 350.0f, 250.0f, Play_Screen_Text, 0.5f);

	//Textbox texbox("res/ButtonBackground/whitePlain.jpg",2, 300.0f, -215.0f, 0.0f, 90, 15, 1);
	//Textbox texbox("res/ButtonBackground/lemonYellow.jpg",2, 300.0f, -215.0f, 0.0f, 90, 15, 1);
	Textbox texbox("res/ButtonBackground/whitePlain.jpg","res/ButtonBackground/lightGreenPlain.jpg",2,3, 300.0f, -215.0f, 0.0f, 90, 15, 1);
	
	

	PokerEngineEvents poker;


	
	state = init_e;
	
	irrklang::ISound* title_theme = SoundEngine->play2D("res/Music/TitleTheme.wav", true, true, true, irrklang::ESM_AUTO_DETECT, true);
	title_theme->setVolume(0.15f);
	title_theme->setIsPaused(false);

	//thr.join();
	
	while (!glfwWindowShouldClose(mainscreen.window))
	{


		//MainScreen::processInput(mainscreen.window);
		glClearColor(0, 0, 0, 0);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINES| GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		if (context == -1)
		{
			context = mainscreen.run();
			if (title_theme->getIsPaused())
				title_theme->setIsPaused(false);
		}
				
				
				
		else {			
			
			//glEnable(GL_DEPTH_TEST);
			if (!title_theme->getIsPaused())
				title_theme->setIsPaused(true);

			
			model = glm::mat4(1.0f);
			//model = glm::translate(model, glm::vec3(-0.5f, 0.5f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(400, 300, 1));
			imageShader.use();
			imageShader.setMatrix4fv("model", model);

			if (poker.button_pos == 1) {
				tex_play_back.bind();
				tex_play_back.render(imageShader);
			}
			else {
				tex_play_back2.bind();
				tex_play_back2.render(imageShader);
			}

			
			
			

			
			

			
			


			if (backButton.render_button(textShader, Play_Screen_Text) == 0)
			{
				context = -1;
				state = init_e;
				
			}


			
			if (state == init_e) {

				poker.init();
				texbox.clear_textbox();
				texbox.clicked_inside_box = false;
				Card_asset.animate_angles[flop] = Card_asset.animate_angles[turn] = Card_asset.animate_angles[river] = Card_asset.animate_angles[p_cards] = Card_asset.animate_angles[c_cards] = 0.0f;
				animate_state = true;
				state++;
				
			}

			else if (state == pre_flop_e)
			{
				poker.pre_flop(state);
				animate_state = true;
				hr_state = -100;
				
			}
				
			else if (state == pre_flop_rob_e)
			{
				
				if (poker.rob_init == 0)
				{
					
					clicked_value = -1;
					poker.both_checked = 0;
					poker.raised = 0;
					poker.amt_raised = 0;
					poker.amt_called = 0;
					poker.amt_input = 0;
					poker.rob_init = 1;
					poker.amt_raised += poker.smallblind;
					poker.act_first = poker.button_pos;
					if (poker.all_in == -1 || poker.all_in == 10)//weird edge case where the big blind is all in and has less than 1bb but, greater than 1sb, but still have to ask the small blind for call or fold
					{
						
						poker.round_of_betting(texbox, state);
						
						
					}
					if (poker.all_in != -1 && ((poker.ring_players[0].stack==0&&poker.ring_players[0].amount_bet<= poker.ring_players[1].amount_bet)||
						(poker.ring_players[1].stack == 0 && poker.ring_players[1].amount_bet <= poker.ring_players[0].amount_bet))&&poker.pot>0)
					{
						//std::cout << "State: " << state << std::endl;
						if(state== pre_flop_rob_e)
							state++;
						poker.rob_init = 0;
						
						continue;
					}

				}
			
				animate_state = false;
				if (clicked_value > -1)
				{
					//if (poker.all_in == -1 || poker.all_in == 10)//weird edge case where the big blind is all in and has less than 1bb but, greater than 1sb, but still have to ask the small blind for call or fold
					//{
					
						poker.act_first = 1;
						poker.round_of_betting(texbox, state);
					//}


				}
				

			}
			else if (state == flop_rob_e)
			{
				
				if (poker.rob_init == 0)
				{
					
					clicked_value = -1;
					poker.both_checked = 0;
					poker.raised = 0;
					poker.amt_raised = 0;
					poker.amt_called = 0;
					poker.amt_input = 0;
					poker.rob_init = 1;
					poker.amt_raised += 0;
					poker.act_first = poker.button_other;
					if (poker.all_in == -1 )//weird edge case where the big blind is all in and has less than 1bb but, greater than 1sb, but still have to ask the small blind for call or fold
					{
						
						poker.round_of_betting(texbox, state);
						
					}
					poker.cards_on_table.emplace_back(poker.deck[7]);
					std::cout << "No. of Cards Turn: " << poker.cards_on_table.size() << std::endl;;
					std::cout << "Table Cards: ";
					for (int z = 0; z < poker.cards_on_table.size(); z++)
					{
						std::cout << std::get<0>(poker.cards_on_table[z]) << " " << std::get<1>(poker.cards_on_table[z]) << " ";
					}
					std::cout << std::endl;
					
				}
				if (poker.all_in != -1 && ((poker.ring_players[0].stack == 0 && poker.ring_players[0].amount_bet <= poker.ring_players[1].amount_bet) ||
					(poker.ring_players[1].stack == 0 && poker.ring_players[1].amount_bet <= poker.ring_players[0].amount_bet)) && poker.pot > 0)
				{
					std::cout << "State: " << state << std::endl;
					if(state== flop_rob_e)
						state++;
					poker.rob_init = 0;
					
					continue;
				}
				
				animate_state = false;
				if (clicked_value > -1)
				{
					//if (poker.all_in == -1 || poker.all_in == 10)
					//{
					poker.act_first = 1;
					poker.round_of_betting(texbox, state);
					//}


				}


			}
			
			else if (state == turn_rob_e)
			{

				if (poker.rob_init == 0)
				{
					clicked_value = -1;
					poker.both_checked = 0;
					poker.raised = 0;
					poker.amt_raised = 0;
					poker.amt_called = 0;
					poker.amt_input = 0;
					poker.rob_init = 1;
					poker.amt_raised += 0;
					poker.act_first = poker.button_other;


					poker.cards_on_table.emplace_back(poker.deck[8]);
					std::cout << "No. of Cards River: " << poker.cards_on_table.size() << std::endl;;
					std::cout << "Table Cards: ";
					for (int z = 0; z < poker.cards_on_table.size(); z++)
					{
						std::cout << std::get<0>(poker.cards_on_table[z]) << " " << std::get<1>(poker.cards_on_table[z]) << " ";
					}

					std::cout << std::endl;


					if (poker.all_in == -1)//weird edge case where the big blind is all in and has less than 1bb but, greater than 1sb, but still have to ask the small blind for call or fold
					{
						
						poker.round_of_betting(texbox, state);
					}
					

				}
				if (poker.all_in != -1 && ((poker.ring_players[0].stack == 0 && poker.ring_players[0].amount_bet <= poker.ring_players[1].amount_bet) ||
					(poker.ring_players[1].stack == 0 && poker.ring_players[1].amount_bet <= poker.ring_players[0].amount_bet)) && poker.pot > 0)
				{
					std::cout << "State: " << state << std::endl;
					if(state== turn_rob_e)
						state++;
					poker.rob_init = 0;
					
					continue;
				}
				
				animate_state = false;
				if (clicked_value > -1)
				{
					//if (poker.all_in == -1 || poker.all_in == 10)
					//{
					poker.act_first = 1;
					poker.round_of_betting(texbox, state);
					//}


				}


			}
			
			else if (state == river_rob_e)
			{

				if (poker.rob_init == 0)
				{
					clicked_value = -1;
					poker.both_checked = 0;
					poker.raised = 0;
					poker.amt_raised = 0;
					poker.amt_called = 0;
					poker.amt_input = 0;
					poker.rob_init = 1;
					poker.amt_raised += 0;
					poker.act_first = poker.button_other;
					//std::cout << "No. of Cards: " << poker.cards_on_table.size() << std::endl;;
					if (poker.all_in == -1 )//weird edge case where the big blind is all in and has less than 1bb but, greater than 1sb, but still have to ask the small blind for call or fold
					{
						
						poker.round_of_betting(texbox, state);
					}
					
				}
				if (poker.all_in != -1 && ((poker.ring_players[0].stack == 0 && poker.ring_players[0].amount_bet <= poker.ring_players[1].amount_bet) ||
					(poker.ring_players[1].stack == 0 && poker.ring_players[1].amount_bet <= poker.ring_players[0].amount_bet)) && poker.pot > 0)
				{
					std::cout << "State: " << state << std::endl;
					if(state== river_rob_e)
						state++;
					poker.rob_init = 0;
					
					continue;
				}
				
				animate_state = false;
				if (clicked_value > -1)
				{
					//if (poker.all_in == -1 || poker.all_in == 10)
					
					poker.act_first = 1;
					poker.round_of_betting(texbox, state);
					//}


				}


				}
			else if (state == settlement_e)
			{
				poker.button_states[0] = poker.button_states[1]= poker.button_states[2]= poker.button_states[3]=0;
				poker.pot_settlement();
				Card_asset.animate_angles[flop] = Card_asset.animate_angles[turn] = Card_asset.animate_angles[river] = Card_asset.animate_angles[p_cards] = Card_asset.animate_angles[c_cards] = 0.0f;
				
				continue;
			}
			else if (state==game_over_e)
			{
				if (gameOver_state == 1)
				{
					Play_Screen_Text.RenderText(textShader, "YOU WIN", 250.0f, 300.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
				}
				else {
					Play_Screen_Text.RenderText(textShader, "YOU LOSE", 250.0f, 300.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
				}
				if (retryButton.render_button(textShader, Play_Screen_Text) == 0)
				{
					state = init_e;
				}
			}


		


			if ((state == flop_animate_e || state == turn_animate_e || state == river_animate_e||state== pre_flop_animate_e ||state ==reveal_e) && state != hr_state)
			{
				if (state < flop_animate_e && state >pre_flop_e)
				{
					
					if (std::get<1>(poker.ring_players[1].cards[0]) == std::get<1>(poker.ring_players[1].cards[1]))
						poker.user_rank = 2;
					else
						poker.user_rank = 1;
					hr_state = state;
				}
				else if (state == reveal_e)
				{
					input_cards = poker.cards_on_table;
					input_cards.emplace_back(poker.ring_players[0].cards[0]);
					input_cards.emplace_back(poker.ring_players[0].cards[1]);
					place_hold = ranking_algorithm(input_cards);
					poker.cpu_rank = place_hold.strength;
					input_cards.clear();
					hr_state = state;

				}
				else if(state>=flop_animate_e) {
					input_cards = poker.cards_on_table;
					input_cards.emplace_back(poker.ring_players[1].cards[0]);
					input_cards.emplace_back(poker.ring_players[1].cards[1]);
					place_hold = ranking_algorithm(input_cards);
					poker.user_rank = place_hold.strength;
					input_cards.clear();
					hr_state = state;
				}
				
			}

			









			if (!animate_state && state != game_over_e)
			{
				texbox.render(imageShader, Play_Screen_Text, textShader);

				

				if (poker.button_states[check] == 1)
				{
					checkButton.but_status = true;
					if (checkButton.render_button(textShader, Play_Screen_Text) + 1)
					{
						clicked_value = check;
					}
				}
				else {
					checkButton.but_status = false;
				}

				if (poker.button_states[call] == 1)
				{
					callButton.but_status = true;
					if (callButton.render_button(textShader, Play_Screen_Text) + 1)
					{
						clicked_value = call;
					}
				}
				else {
					callButton.but_status = false;
				}


				if (poker.button_states[raise] == 1)
				{
					raiseButton.but_status = true;
					if (raiseButton.render_button(textShader, Play_Screen_Text) + 1)
					{
						clicked_value = raise;
					}
				}
				else {
					raiseButton.but_status = false;
				}


				if (poker.button_states[fold] == 1)
				{
					foldButton.but_status = true;
					if (foldButton.render_button(textShader, Play_Screen_Text) + 1)
					{
						clicked_value = fold;
					}
				}
				else {
					foldButton.but_status = false;
				}




			}

			
			





			




			if (state != game_over_e)
			{
				Model_shader.use();


				if (state >= pre_flop_animate_e && poker.folded == -1)
					Card_asset.animate_player_cards(Model_shader, true, poker, state);
				if (state >= pre_flop_animate_e && state != reveal_e && poker.folded == -1)
					Card_asset.animate_cpu_cards(Model_shader, false, poker, state);
				else if(state==reveal_e && poker.folded==-1)
				{
					Card_asset.animate_cpu_cards(Model_shader, true, poker, state);
				
					Play_Screen_Text.RenderText(textShader, ranknames[poker.cpu_rank-1], 230.0f, 550.0f, 0.20f, glm::vec3(1.0f, 1.0f, 0.0f));
					Model_shader.use();

				}				

				if (state >= flop_animate_e && poker.folded == -1)
					Card_asset.animate_flop(Model_shader, true, poker, state);
				if (state >= turn_animate_e && poker.folded == -1)
					Card_asset.animate_turn(Model_shader, true, poker, state);
				if (state >= river_animate_e && poker.folded == -1)
					Card_asset.animate_river(Model_shader, true, poker, state);

				Play_Screen_Text.RenderText(textShader, "Pot: " + std::to_string(poker.pot), 200.0f, 250.0f, 0.22f, glm::vec3(1.0f, 1.0f, 1.0f));	//render pot
				Play_Screen_Text.RenderText(textShader, "Bet: " + std::to_string(poker.ring_players[0].amount_bet), 510.0f, 450.0f, 0.22f, glm::vec3(1.0f, 1.0f, 1.0f));	//render amt_bet by CPU
				Play_Screen_Text.RenderText(textShader, "Bet: " + std::to_string(poker.ring_players[1].amount_bet), 510.0f, 250.0f, 0.22f, glm::vec3(1.0f, 1.0f, 1.0f));	//render amt_bet by User
				
				//render Previuos CPU Action??
				//render Previuos User Action??

				for (int u = 0; u < 9; u++)
				{
					if (u == poker.user_rank - 1)
					{
						Play_Screen_Text.RenderText(textShader, ranknames[u], 25.0f, 25.0f + u * 20.f, 0.20f, glm::vec3(0.0f, 1.0f, 0.0f));
					}
					else {
						Play_Screen_Text.RenderText(textShader, ranknames[u], 25.0f, 25.0f + u * 20.f, 0.20f, glm::vec3(1.0f, 1.0f, 1.0f));
					}

				}



			}
			Play_Screen_Text.RenderText(textShader, std::to_string(poker.ring_players[0].stack), 418.0f, 515.0f, 0.22f, glm::vec3(1.0f, 1.0f, 1.0f));	//render CPU stack
			Play_Screen_Text.RenderText(textShader, std::to_string(poker.ring_players[1].stack), 418.0f, 84.0f, 0.22f, glm::vec3(1.0f, 1.0f, 1.0f));	//render User stack
			
					
			

			

			

			
			
		}
		

		glfwSwapBuffers(mainscreen.window);

		glfwPollEvents();

	}

	mainscreen.destroy();

//}//Heap-allocate things

	glfwTerminate();
	return 0;
}







