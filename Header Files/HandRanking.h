#pragma once
#include<iostream>
#include<vector>
#include<tuple>
#include <string>




struct Hand_and_strength
{
	std::vector<std::tuple<unsigned int, unsigned int>> hand;
	unsigned int strength;
};

//Hand_and_strength ranking_algorithm(std::vector<std::tuple<unsigned int, unsigned int>>);




Hand_and_strength ranking_algorithm(std::vector<std::tuple<unsigned int, unsigned int>> board)
{

	enum suits { spades, hearts, diamonds, clubs };
	enum cards { ace_one, duece, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace };
	int no_of_suits = 4;
	int no_of_cards = 14;

	Hand_and_strength best_det;


	int flush_var = -1;
	int straight_count[2][3] = { {0,100,-1},{0,100,-1} };
	int flush_straight_count[4][15] = { 0 };

	std::vector<std::vector<int>> same_card_count = { {},{},{} };
	std::vector<std::vector<int>> res_count = { {0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0} };







	for (int iter_card = 0; iter_card < board.size(); iter_card++)
	{
		if (std::get<1>(board[iter_card]) == ace_one)
		{
			res_count[ace][0]++;
			res_count[ace].push_back(std::get<0>(board[iter_card]));
			flush_straight_count[std::get<0>(board[iter_card])][ace + 1]++;

		}
		res_count[std::get<1>(board[iter_card])][0]++;
		res_count[std::get<1>(board[iter_card])].push_back(std::get<0>(board[iter_card]));
		flush_straight_count[std::get<0>(board[iter_card])][0]++;
		flush_straight_count[std::get<0>(board[iter_card])][std::get<1>(board[iter_card]) + 1]++;

	}


	for (int flush_iter = 0; flush_iter < no_of_suits; flush_iter++)
		if (flush_straight_count[flush_iter][0] > 4)
			flush_var = flush_iter;



	for (int iter_strt = 0; iter_strt < no_of_cards; iter_strt++)
	{

		//Counting for Quads,Trips,Two Pairs, Pairs
		if (res_count[iter_strt][0] == 4 && iter_strt != 0)
			same_card_count[0].push_back(iter_strt);
		else if (res_count[iter_strt][0] == 3 && iter_strt != 0)
			same_card_count[1].push_back(iter_strt);
		else if (res_count[iter_strt][0] == 2 && iter_strt != 0)
			same_card_count[2].push_back(iter_strt);



		//Checking for Straights
		if (res_count[iter_strt][0] != 0)
		{
			straight_count[0][0]++;

			if (straight_count[0][2] <= iter_strt && straight_count[0][0] > 4)
			{
				if (straight_count[0][1] == 100)
				{
					straight_count[0][1] = 4;

				}
				straight_count[0][2] = iter_strt;
				straight_count[0][1]++;


			}
		}

		else
		{
			straight_count[0][0] = 0;
		}


		//Checking for Straight Flush
		if (flush_var != -1)
		{
			if (flush_straight_count[flush_var][iter_strt + 1] != 0)
			{
				straight_count[1][0]++;
				if (straight_count[1][2] <= iter_strt && straight_count[1][0] > 4)
				{
					if (straight_count[1][1] == 100)
					{
						straight_count[1][1] = 4;

					}
					straight_count[1][2] = iter_strt;
					straight_count[1][1]++;
				}
			}

			else
			{
				straight_count[1][0] = 0;
			}

		}
	}


	if (straight_count[1][1] != 100)
	{

		std::cout << "Straight Flush" << std::endl;
		for (int z = straight_count[1][2]; best_det.hand.size() < 5; z--)
			best_det.hand.push_back(std::make_tuple(flush_var, z));
		//Done
		best_det.strength = 9;
	}
	else if (!same_card_count[0].empty())
	{
		std::cout << "Four of a kind" << std::endl;
		for (int z = 0; z < 4; z++)
			best_det.hand.push_back(std::make_tuple(z, same_card_count[0][0]));
		for (int z = no_of_cards - 1; best_det.hand.size() < 5; z--)
		{
			if (z != same_card_count[0][0] && res_count[z][0] != 0)
				best_det.hand.push_back(std::make_tuple(res_count[z][1], z));
		}
		//Done
		best_det.strength = 8;
	}
	else if ((same_card_count[1].size() > 1) || (same_card_count[1].size() > 0 && same_card_count[2].size() > 0))
	{
		std::cout << "Full House" << std::endl;
		for (int z = 0; z < 3; z++)
			best_det.hand.push_back(std::make_tuple(res_count[same_card_count[1].back()][z + 1], same_card_count[1].back()));
		for (int k = no_of_cards - 1; k > 0 and best_det.hand.size() < 5; k--) // greater than zero because the ACE check is already done
		{
			if (res_count[k][0] > 1 && k != same_card_count[1].back())
			{
				best_det.hand.push_back(std::make_tuple(res_count[k][1], k));
				best_det.hand.push_back(std::make_tuple(res_count[k][2], k));
			}
		}
		best_det.strength = 7;
		//Done
	}
	else if (flush_var != -1)
	{
		std::cout << "Flush" << std::endl;
		for (int z = no_of_cards; best_det.hand.size() < 5; z--)
			if (flush_straight_count[flush_var][z] != 0)
				best_det.hand.push_back(std::make_tuple(flush_var, z));
		//Done
		best_det.strength = 6;
	}
	else if (straight_count[0][1] != 100)
	{
		std::cout << "Straight" << std::endl;
		for (int z = straight_count[0][2]; best_det.hand.size() < 5; z--)
			best_det.hand.push_back(std::make_tuple(res_count[z][1], z));
		//Done
		best_det.strength = 5;
	}
	else if (same_card_count[1].size() > 0)
	{
		std::cout << "Three of a kind" << std::endl;
		for (int z = 0; z < 3; z++)
		{
			best_det.hand.push_back(std::make_tuple(res_count[same_card_count[1][0]][z + 1], same_card_count[1][0]));
		}
		for (int k = no_of_cards - 1; best_det.hand.size() < 5; k--)
		{
			if (k != same_card_count[1][0] && res_count[k][0] != 0)
				best_det.hand.push_back(std::make_tuple(res_count[k][1], k));
		}
		//Done
		best_det.strength = 4;
	}
	else if (same_card_count[2].size() >= 2)
	{
		std::cout << "Two Pair" << std::endl;
		for (int z = 0; z < 2; z++)
		{
			best_det.hand.push_back(std::make_tuple(res_count[same_card_count[2][same_card_count[2].size() - 1]][z + 1], same_card_count[2][same_card_count[2].size() - 1]));
			//best_hand.push_back(std::make_tuple(res_count[same_card_count[2][1]][z + 1], same_card_count[2][1]));
		}
		for (int z = 0; z < 2; z++)
		{
			best_det.hand.push_back(std::make_tuple(res_count[same_card_count[2][same_card_count[2].size() - 2]][z + 1], same_card_count[2][same_card_count[2].size() - 2]));
		}
		for (int k = no_of_cards - 1; best_det.hand.size() < 5; k--)
		{
			if (k != same_card_count[2][same_card_count[2].size() - 2] && k != same_card_count[2][same_card_count[2].size() - 1] && res_count[k][0] != 0)
				best_det.hand.push_back(std::make_tuple(res_count[k][1], k));
		}
		//Done
		best_det.strength = 3;
	}
	else if (same_card_count[2].size() == 1)
	{
		std::cout << "One Pair" << std::endl;
		for (int z = 0; z < 2; z++)
		{
			best_det.hand.push_back(std::make_tuple(res_count[same_card_count[2][0]][z + 1], same_card_count[2][0]));
		}
		for (int k = no_of_cards - 1; best_det.hand.size() < 5; k--)
		{
			if (k != same_card_count[2][0] && res_count[k][0] != 0)
				best_det.hand.push_back(std::make_tuple(res_count[k][1], k));
		}
		//done
		best_det.strength = 2;
	}
	else
	{
		std::cout << "High Card" << std::endl;
		for (int k = no_of_cards - 1; best_det.hand.size() < 5; k--)
		{
			if (res_count[k][0] != 0)
				best_det.hand.push_back(std::make_tuple(res_count[k][1], k));
		}
		//Done
		best_det.strength = 1;
	}


	if (best_det.hand.size() != 5)
	{
		std::cout << "Error in Logic, best hand size not equal to 5" << std::endl;
	}

	std::cout << "The Best Hand:" << std::endl;
	for (int best_hand_iter = 0; best_hand_iter < best_det.hand.size(); best_hand_iter++)
		std::cout << std::get<0>(best_det.hand[best_hand_iter]) << " " << std::get<1>(best_det.hand[best_hand_iter]) << std::endl;

	return best_det;

}
