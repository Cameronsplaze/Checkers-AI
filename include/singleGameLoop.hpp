#ifndef SINGLE_GAME_LOOP_HPP_INCLUDED
#define SINGLE_GAME_LOOP_HPP_INCLUDED

#include <bitset>				// std::bitset
#include <memory> 				// std::shared_ptr

#include "player.hpp"			// class Player;
#include "checkerboard.hpp"		// stringToBoard();
#include "globalconsts.hpp"

std::bitset<96> getStartBoard();

class Game
{
public:
	Game(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2, 				   // the players
		 const bool &useGUI=false,						   // if you want a gui
		 std::bitset<96> start_board = getStartBoard());   // what the default board looks like (for testing)
	
private:
	std::shared_ptr<Player> player1_;
	std::shared_ptr<Player> player2_;
	const bool useGUI_;
};


#endif // SINGLE_GAME_LOOP_HPP_INCLUDED