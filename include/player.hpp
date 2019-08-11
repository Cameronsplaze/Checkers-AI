#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <SFML/Graphics.hpp>

#include <string> // string
#include <iostream> // cout endl
#include <memory> // unique_ptr
#include <vector> // vector
#include <bitset> // bitset
#include <thread> // sleep_for
#include <chrono> // chrono_literals;

#include "globalconsts.hpp"
#include "checkerboard.hpp"
#include "negamax.hpp"

class SpriteChecker
{
public:
	sf::Sprite sprite;
	bool isRed = false;
	bool king = false;
};

class Player
{
public:
	Player(const std::string &plr_type, bool redTeam, sf::RenderWindow &window);
	std::bitset<96> getMove(std::vector<std::unique_ptr<SpriteChecker>> &startBoard);

private:
	void humanManager(std::vector<std::unique_ptr<SpriteChecker>> &checkers);
	int MousePositionToInt(sf::Vector2i locationClicked);
	void swapCheckers(int f, int s, std::unique_ptr<SpriteChecker> &swapStorage, std::vector<std::unique_ptr<SpriteChecker>> &checkers);
	bool JumpingRecursively(int pieceIndex, std::vector<std::unique_ptr<SpriteChecker>> &checkers);

	sf::RenderWindow &window_;
	std::string playerType_;
	const bool isRedTeam_;
};

std::bitset<96> checkersToBoard(std::vector<std::unique_ptr<SpriteChecker>> &checkers);
const sf::Vector2f intToCords(uint i);

#endif