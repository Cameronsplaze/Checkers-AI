#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <iostream>		// std::cout, std::endl
#include <string> 		// std::string
#include <bitset>		// std::bitset
#include <chrono> 		// std::chrono_literals
#include <thread> 		// std::this_thread::sleep_for
#include <memory>		// std::shared_ptr

#include "checkerboard.hpp"
#include "negamax.hpp"
#include "managerSFML.hpp"

class Player
{
public:
	Player(const std::string &plr_type);
	void setGUI(std::shared_ptr<GUI> &gameGUI);
	std::bitset<96> getMove(const std::bitset<96> &bitBoard, bool isRedTeam);
	
private:	
	bool validMovesRecursive(int firstCheckerID, const std::bitset<96> &oldMove, bool isRedTeam);

private:
	const std::string playerType_;
	std::shared_ptr<GUI> gameGUI_;
};

#endif
