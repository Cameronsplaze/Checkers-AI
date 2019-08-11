#ifndef MANAGER_SFML_HPP_INCLUDED
#define MANAGER_SFML_HPP_INCLUDED


#include <SFML/Graphics.hpp>
#include <iostream> // cout, endl
#include <string> // string
#include <vector> // vector
#include <memory> // unique_ptr
#include <bitset> // bitset
#include <algorithm> // find, swap

#include "globalconsts.hpp"
#include "player.hpp"



class CheckerboardGUI
{
public:
	CheckerboardGUI(const std::string &player1, const std::string &player2);

private:
	void updateSprites(std::bitset<96> startBoard);
	void windowUpdate();
	bool loadTextures();
	void mainGameloop();


	//players
	Player player1_;
	Player player2_;

	bool isPlayer1Turn;
	// Textures
	sf::Texture checkerboardTexture_;
	sf::Texture whiteCheckerTexture_;
	sf::Texture whiteCheckerKingTexture_;
	sf::Texture blackCheckerTexture_;
	sf::Texture blackCheckerKingTexture_;

	// Sprites
	std::vector<std::unique_ptr<SpriteChecker>> checkers_;
	sf::Sprite checkerboard_;

	//window
	sf::RenderWindow window_; // Starting with 648 since that's default width/height of checkerboard
};




#endif