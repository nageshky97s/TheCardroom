#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<tuple>
#include<random>
#include<numeric>
#include<stdlib.h>

#include "HandRanking.h"
#include "Global_objects.h"
#include "Textbox.h"

class PokerEngineEvents {

public:

	struct player {
		unsigned int stack;
		std::vector<std::tuple<unsigned int, unsigned int >> cards;
		int table_pos;
		unsigned int amount_bet;

	};


	
	unsigned int smallblind;
	unsigned int bigblind;
	unsigned int button_pos;
	unsigned int button_other;
	unsigned int pot;
	int folded, all_in;
	

	std::vector<std::tuple<unsigned int, unsigned int >> deck;
	std::vector<std::tuple<unsigned int, unsigned int >> cards_on_table;

	std::vector<player> ring_players;


	unsigned int act_first;
	int button_states[4];
	unsigned int both_checked ;
	unsigned int raised ;
	unsigned int amt_raised ;
	unsigned int amt_called ;
	unsigned int amt_input ;

	int rob_init;
	unsigned int cpu_rank;
	unsigned int user_rank;

	PokerEngineEvents() {

		player Computer, User;
		Computer.table_pos = 1;
		User.table_pos = 2;

		
		smallblind = 25;
		bigblind = 50;
		button_pos = 1;
		button_other = 0;
		
		//Computer.stack = 980;
		//User.stack = 20;
		//Computer.stack = 20;
		//User.stack = 980;
		Computer.stack = User.stack = bigblind * 20;

		for (unsigned int i = 0; i < 4; i++)
		{
			button_states[i] = 0;
			for (unsigned int j = 0; j < 13; j++)
			{
				deck.emplace_back(std::make_tuple(i, j));

			}
		}
		ring_players.emplace_back(Computer);
		ring_players.emplace_back(User);



		//act_first = 1;

		 both_checked = 0;
		 raised = 0;
		 amt_raised = 0;
		 amt_called = 0;
		 amt_input = 0;
		 rob_init = 0;
		 cpu_rank = 0;
		 user_rank = 0;
	}

	void init() {

		button_pos = 1;
		button_other = 0;
		ring_players[0].stack = ring_players[1].stack = bigblind * 20;
		//ring_players[0].stack = 980;
		//ring_players[1].stack = 20;
		//ring_players[0].stack = 20;
		//ring_players[1].stack = 980;
		both_checked = 0;
		raised = 0;
		amt_raised = 0;
		amt_called = 0;
		amt_input = 0;
		rob_init = 0;
		


		if(cards_on_table.size()!=0)
			cards_on_table.clear();
		if(ring_players[0].cards.size()!=0)
			ring_players[0].cards.clear();
		if(ring_players[1].cards.size() != 0)
			ring_players[1].cards.clear();


		ring_players[0].amount_bet = ring_players[1].amount_bet = 0;
		button_states[0] = button_states[1] = button_states[2] = button_states[3] = 0;
		cpu_rank = 0;
		user_rank = 0;

	}
	void pre_flop(int& state)
	{
		folded = all_in = -1;
		std::shuffle(std::begin(deck), std::end(deck), rng);
		//card-distribution animation and button movement animation
		pot = 0;


		if (ring_players[button_pos].stack < smallblind)
		{
			all_in = button_pos;
			pot = ring_players[button_pos].stack * 2;
			ring_players[button_pos].amount_bet = ring_players[button_other].amount_bet = ring_players[button_pos].stack;
			ring_players[button_pos].stack -= ring_players[button_pos].stack;
			ring_players[button_other].stack -= pot / 2;


		}
		else if (ring_players[button_other].stack < bigblind) {

			//when the remaining stack is less 1bb and 1sb
			if (ring_players[button_other].stack < smallblind)
			{
				all_in = button_other;
				pot = ring_players[button_other].stack * 2;
				ring_players[button_pos].amount_bet = ring_players[button_other].amount_bet = ring_players[button_other].stack;
				ring_players[button_other].stack -= ring_players[button_other].stack;
				ring_players[button_pos].stack -= pot / 2;

			}
			//when the remaining stack is less than 1bb but greater than 1sb
			else {
				all_in = 10;
				pot = ring_players[button_other].stack + smallblind;
				ring_players[button_pos].stack -= smallblind;
				ring_players[button_other].stack -= ring_players[button_other].stack;
				ring_players[button_pos].amount_bet = smallblind;
				ring_players[button_other].amount_bet = pot - smallblind;

			}

		}
		else {
			ring_players[0].stack = ring_players[0].stack - smallblind;
			ring_players[1].stack = ring_players[1].stack - smallblind;
			ring_players[button_other].stack = ring_players[button_other].stack - smallblind;
			ring_players[0].amount_bet = ring_players[1].amount_bet = smallblind;
			ring_players[button_other].amount_bet += smallblind;
			pot = bigblind + smallblind;

		}

		for (int i = 0; i < 2; i++)
		{
			ring_players[button_pos].cards.emplace_back(deck[0 + i]);
			ring_players[button_other].cards.emplace_back(deck[2 + i]);
			cards_on_table.emplace_back(deck[4 + i]);

		}
		cards_on_table.emplace_back(deck[6]);
		
		std::cout << "Your Cards:" << std::endl << std::get<0>(ring_players[1].cards[0]) << " " << std::get<1>(ring_players[1].cards[0]) << std::endl;
		std::cout << std::get<0>(ring_players[1].cards[1]) << " " << std::get<1>(ring_players[1].cards[1]) << std::endl;
		std::cout << "CPU Cards:" << std::endl << std::get<0>(ring_players[0].cards[0]) << " " << std::get<1>(ring_players[0].cards[0]) << std::endl;
		std::cout << std::get<0>(ring_players[0].cards[1]) << " " << std::get<1>(ring_players[0].cards[1]) << std::endl;

		state++;
		std::cout << state << std::endl;

	}




	void round_of_betting(Textbox& tb, int& state) {
		unsigned int first = act_first;
		unsigned int second;
		
		int percent;
		int action;
		std::string value_holder;

		if (first == 1)
			second = 0;
		else
			second = 1;
		

		while(true)
		{
			action = -1;
			if (ring_players[first].amount_bet == ring_players[second].amount_bet)
			{
				if (first == 0)
				{
					//choose between check and raise, so two options
					action = dist2(rng);
					std::cout << "0.check 2.raise" << std::endl;

					if (action == 0)
					{
						action = check;

					}
					else {
						action = raise;
					}
					std::cout << "Action Selected by the CPU: " << action << std::endl;
				}
				else {
					button_states[check] = button_states[raise]= 1;
					button_states[call] = button_states[fold] = 0;
					std::cout << "0.check 2.raise" << std::endl;
					if (clicked_value > -1)
					{
						action = clicked_value;
						clicked_value = -1;
					}
					else {
						
						return;
					}
					std::cout << "Action chooosen by the USER: " << action << std::endl;
				}
				//check

				if (action == check)
				{
					both_checked++;


					if (both_checked % 2 == 0 || state==pre_flop_rob_e)// because once the sb call and bb checks round of betting is over and should move on to the next change
					{
						state++;
						rob_init = 0;
						break;
						
					}
				}
				//raise
				else if (action == raise)
				{
					raised++;
					if (first == 0)
					{
						percent = dist6(rng);
						if (percent == 0)
						{
							amt_input = (int)(0.25 * pot);
						}
						else if (percent == 1)
						{
							amt_input = (int)(0.50 * pot);
						}
						else if (percent == 2)
						{
							amt_input = (int)(0.75 * pot);
						}
						else if (percent == 3)
						{
							amt_input = pot;
						}
						else if (percent == 4)
						{
							amt_input = pot * 2;
						}
						else if (percent == 5)
						{
							amt_input = 10000;
						}
						
						std::cout << "The amount raised by the CPU: " << amt_input << std::endl;
					}
					else {
						std::cout << "Enter raise amount" << std::endl;
						value_holder = tb.textbox_value();
						if (value_holder.empty())
							return;
						else
							amt_input = std::stoi(value_holder);
						tb.clear_textbox();
						std::cout << "The amount raised: " << amt_input << std::endl;
					}
					amt_raised += amt_input;
					if (amt_raised >= ring_players[first].stack)
					{
						amt_raised = ring_players[first].stack;
						all_in++;

					}
					pot += amt_raised;
					ring_players[first].stack -= amt_raised;
					ring_players[first].amount_bet += amt_raised;


				}


			}



			else if (ring_players[first].amount_bet < ring_players[second].amount_bet)
				//else
			{
				
				if (all_in != -1 || abs((int)ring_players[first].amount_bet - (int)ring_players[second].amount_bet) >= (int)ring_players[first].stack)
				{
					
					if (first == 0)
					{
						action = dist2(rng);
						std::cout << "1.call 3.fold" << std::endl;
						if (action == 0)
						{
							action = call;
						}
						else if (action == 1)
						{
							action = fold;
						}
						std::cout << "Action Selected by the CPU: " << action << std::endl;
					}
					else {
						button_states[call] = button_states[fold] = 1;
						button_states[check] = button_states[raise] = 0;
						std::cout << "1.call  3.fold" << std::endl;

						if (clicked_value > -1)
						{
							action = clicked_value;
							clicked_value = -1;
						}
						else {
							return;
						}
						std::cout << "Action Value: " << action << std::endl;
					}
				}

				else {
					if (first == 0)
					{
						//raise, call and fold are the two options to consider from 
						action = dist3(rng);
						std::cout << "1.call 2.raise 3.fold" << std::endl;
						//std::cout << "Action Selected by the CPU: " << action << std::endl;
						if (action == 0)
						{
							action = call;
						}
						else if (action == 1)
						{
							action = raise;
						}
						else {
							action = fold;
						}
						std::cout << "Action Selected by the CPU: " << action << std::endl;
					}
					else {
						button_states[call] = button_states[raise] = button_states[fold] = 1;
						button_states[check]=0;
						std::cout << "1.call 2.raise 3.fold" << std::endl;
						std::cout << "Clicked Value: " << clicked_value << std::endl;
						if (clicked_value > -1)
						{
							action = clicked_value;
							clicked_value = -1;
						}
						else {
							
							return;
						}
						std::cout << "Action Value: " << action << std::endl;
						std::cout << "Clicked Value: " << clicked_value << std::endl;
					}

				}


				//raise
				if (action == raise)
				{
					raised++;

					if (first == 0)
					{
						percent = dist6(rng);
						if (percent == 0)
						{
							amt_input = (int)(0.25 * pot);
						}
						else if (percent == 1)
						{
							amt_input = (int)(0.50 * pot);
						}
						else if (percent == 2)
						{
							amt_input = (int)(0.75 * pot);
						}
						else if (percent == 3)
						{
							amt_input = pot;
						}
						else if (percent == 4)
						{
							amt_input = pot * 2;
						}
						else if (percent == 5)
						{
							amt_input = 10000;
						}
						std::cout << "The amount raised by the CPU: " << amt_input << std::endl;
					}
					else {
						std::cout << "Enter raise amount " << std::endl;
						value_holder = tb.textbox_value();
						if (value_holder.empty())
							return;
						else
							amt_input = std::stoi(value_holder);
						tb.clear_textbox();
						std::cout << "Entered Amount: " << amt_input << std::endl;

					}

					amt_raised += amt_input;

					if (amt_raised > ring_players[first].stack)
					{
						amt_raised = ring_players[first].stack;
						all_in++;
					}
					std::cout << "The amount raised : " << amt_raised << std::endl;
					pot += amt_raised;
					ring_players[first].stack -= amt_raised;
					ring_players[first].amount_bet += amt_raised;
				}

				//call
				else if (action == call)
				{
					amt_called = abs((int)ring_players[first].amount_bet - (int)ring_players[second].amount_bet);
					if (amt_called >= ring_players[first].stack)
					{
						amt_called = ring_players[first].stack;
						all_in++;
					}
					pot += amt_called;
					ring_players[first].amount_bet += amt_called;
					ring_players[first].stack -= amt_called;
					std::cout << "Amount Called : " << amt_called << std::endl;

					if (raised || all_in != -1)
					{
						state++;
						rob_init = 0;
						break;
						
					}



				}

				//fold
				else if (action == fold)
				{

					folded = second;// because directly give pot to the player that doesn't fold
					rob_init = 0;
					state=settlement_e;
					return;

				}
			}

			if (first == 1)
			{
				first = 0;
				second = 1;
			}
			else
			{
				first = 1;
				second = 0;
			}


		}


	}

	void pot_settlement() {

		if (folded != -1)
		{
			std::cout << "Player getting the money = " << folded << std::endl;

			if (ring_players[0].amount_bet > ring_players[1].amount_bet)
			{
				ring_players[0].stack += abs((int)ring_players[0].amount_bet - (int)ring_players[1].amount_bet);
			}
			else if (ring_players[0].amount_bet < ring_players[1].amount_bet)
			{
				ring_players[1].stack += abs((int)ring_players[0].amount_bet - (int)ring_players[1].amount_bet);
			}

			ring_players[folded].stack += std::min(ring_players[0].amount_bet, ring_players[1].amount_bet) * 2;


		}
		else {

			cards_on_table.emplace_back(ring_players[0].cards[0]);
			cards_on_table.emplace_back(ring_players[0].cards[1]);
			Hand_and_strength player1 = ranking_algorithm(cards_on_table);
			//cpu_rank = player1.strength-1;
			cards_on_table.resize(5);
			cards_on_table.emplace_back(ring_players[1].cards[0]);
			cards_on_table.emplace_back(ring_players[1].cards[1]);
			Hand_and_strength player2 = ranking_algorithm(cards_on_table);
			user_rank = player2.strength;
			std::cout << "CPU Hand Strength: " << player1.strength << " Player Hand Strength: " << player2.strength << std::endl;

			if (player1.strength > player2.strength)
			{
				if (ring_players[0].amount_bet == ring_players[1].amount_bet)
				{
					ring_players[0].stack += std::min(ring_players[0].amount_bet, ring_players[1].amount_bet) * 2;
				}
				else if (ring_players[0].amount_bet > ring_players[1].amount_bet)
				{
					ring_players[0].stack += std::min(ring_players[0].amount_bet, ring_players[1].amount_bet) * 2;
					ring_players[0].stack += abs((int)ring_players[0].amount_bet - (int)ring_players[1].amount_bet);
				}
				else {
					ring_players[0].stack += std::min(ring_players[0].amount_bet, ring_players[1].amount_bet) * 2;
					ring_players[1].stack += abs((int)ring_players[0].amount_bet - (int)ring_players[1].amount_bet);
				}

			}
			else if (player1.strength < player2.strength)
			{
				if (ring_players[1].amount_bet == ring_players[0].amount_bet)
				{
					ring_players[1].stack += std::min(ring_players[0].amount_bet, ring_players[1].amount_bet) * 2;

				}
				else if (ring_players[1].amount_bet > ring_players[0].amount_bet)
				{
					ring_players[1].stack += std::min(ring_players[0].amount_bet, ring_players[1].amount_bet) * 2;
					ring_players[1].stack += abs((int)ring_players[0].amount_bet - (int)ring_players[1].amount_bet);
				}
				else {
					ring_players[1].stack += std::min(ring_players[0].amount_bet, ring_players[1].amount_bet) * 2;
					ring_players[0].stack += abs((int)ring_players[0].amount_bet - (int)ring_players[1].amount_bet);
				}

			}
			else {

				int flag = 1;
				for (int i = 0; i < 5; i++)
				{
					if (std::get<1>(player1.hand[i]) > std::get<1>(player2.hand[i]))
					{
						if (ring_players[0].amount_bet == ring_players[1].amount_bet)
						{
							ring_players[0].stack += std::min(ring_players[0].amount_bet, ring_players[1].amount_bet) * 2;
						}
						else if (ring_players[0].amount_bet > ring_players[1].amount_bet)
						{
							ring_players[0].stack += std::min(ring_players[0].amount_bet, ring_players[1].amount_bet) * 2;
							ring_players[0].stack += abs((int)ring_players[0].amount_bet - (int)ring_players[1].amount_bet);
						}
						else {
							ring_players[0].stack += std::min(ring_players[0].amount_bet, ring_players[1].amount_bet) * 2;
							ring_players[1].stack += abs((int)ring_players[0].amount_bet - (int)ring_players[1].amount_bet);
						}



						flag = 0;
						break;

					}
					else if (std::get<1>(player1.hand[i]) < std::get<1>(player2.hand[i]))
					{

						if (ring_players[1].amount_bet == ring_players[0].amount_bet)
						{
							ring_players[1].stack += std::min(ring_players[0].amount_bet, ring_players[1].amount_bet) * 2;

						}
						else if (ring_players[1].amount_bet > ring_players[0].amount_bet)
						{
							ring_players[1].stack += std::min(ring_players[0].amount_bet, ring_players[1].amount_bet) * 2;
							ring_players[1].stack += abs((int)ring_players[0].amount_bet - (int)ring_players[1].amount_bet);
						}
						else {
							ring_players[1].stack += std::min(ring_players[0].amount_bet, ring_players[1].amount_bet) * 2;
							ring_players[0].stack += abs((int)ring_players[0].amount_bet - (int)ring_players[1].amount_bet);
						}



						flag = 0;
						break;
					}
				}
				if (flag)
				{
					ring_players[0].stack += std::min(ring_players[0].amount_bet, ring_players[1].amount_bet);
					ring_players[1].stack += std::min(ring_players[0].amount_bet, ring_players[1].amount_bet);

					if (ring_players[0].amount_bet > ring_players[1].amount_bet)
					{
						ring_players[0].stack += abs((int)ring_players[0].amount_bet - (int)ring_players[1].amount_bet);
					}
					else if (ring_players[0].amount_bet < ring_players[1].amount_bet)
					{
						ring_players[1].stack += abs((int)ring_players[0].amount_bet - (int)ring_players[1].amount_bet);
					}
				}

			}


		}


		if (ring_players[0].stack == 0)
		{
			std::cout << "YOU WIN" << std::endl;
			state = game_over_e;
			SoundEngine->play2D(win_sound, false);
			gameOver_state = 1;
			return;
			//break;

		}
		else if (ring_players[1].stack == 0) {
			std::cout << "YOU LOSE" << std::endl;
			SoundEngine->play2D(loss_sound, false);
			state = game_over_e;
			gameOver_state = 0;
			return;
			//break;
		}





		if (button_pos == 1)
		{
			button_pos = 0;
			button_other = 1;
		}
		else
		{
			button_pos = 1;
			button_other = 0;
		}
		
		ring_players[0].amount_bet = ring_players[1].amount_bet = 0;
		cards_on_table.clear();
		ring_players[button_pos].cards.clear();
		ring_players[button_other].cards.clear();	

		state = pre_flop_e;
	}



















	
































	
};