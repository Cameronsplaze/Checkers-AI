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



// #include <SFML/Graphics.hpp>

// #include <string> // string
// #include <iostream> // cout endl
// #include <memory> // unique_ptr
// #include <vector> // vector
// #include <bitset> // bitset
// #include <thread> // sleep_for
// #include <chrono> // chrono_literals;

// #include "globalconsts.hpp"
// #include "checkerboard.hpp"
// #include "negamax.hpp"

// class SpriteChecker
// {
// public:
// 	sf::Sprite sprite;
// 	bool isRed = false;
// 	bool king = false;
// };

// class Player
// {
// public:
// 	Player(const std::string &plr_type);
// 	std::bitset<96> getMove(std::vector<std::unique_ptr<SpriteChecker>> &startBoard);

// private:
// 	void humanManager(std::vector<std::unique_ptr<SpriteChecker>> &checkers);
// 	int bitboard(sf::Vector2i locationClicked);
// 	void swapCheckers(int f, int s, std::unique_ptr<SpriteChecker> &swapStorage, std::vector<std::unique_ptr<SpriteChecker>> &checkers);
// 	bool JumpingRecursively(int pieceIndex, std::vector<std::unique_ptr<SpriteChecker>> &checkers);

// 	// For GUI:
// 	sf::RenderWindow &window_;
// 	sf::Event &event_;
// 	// For Player logic:
// 	std::string playerType_;
// 	const bool isRedTeam_;
// };

// std::bitset<96> checkersToBoard(std::vector<std::unique_ptr<SpriteChecker>> &checkers);
// const sf::Vector2f intToCords(uint i);