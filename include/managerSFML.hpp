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

class SpriteChecker
{
public:
	sf::Sprite sprite;
	bool isRed = false;
	bool king = false;
};

class GUI
{
public:
    GUI();
    // Update what's on screen, with newBoard:
    void setBoard(const std::bitset<96> newBoard);
    // whether some human closed the window:
    bool isWindowOpen();

private:
    // Load the initial textures from the images dir:
    bool loadTextures();
    // Convert ID (0-31) to x,y on checker board:
    const sf::Vector2f intToCords(uint i);
    // Update the checkers_ variable with newBoard:
    void checkersUpdate(const std::bitset<96> newBoard);
    // Update what's on screen, with checkers_:
    void windowUpdate();
    // Process any events that happened since last call:
    void processEvents();

private:
    // Window Stuff:
    sf::RenderWindow window_;

    // Sprite Stuff:
    std::vector<std::unique_ptr<SpriteChecker>> checkers_;
    sf::Sprite checkerboard_;

    // Texture Stuff:
	sf::Texture checkerboardTexture_;
	sf::Texture whiteCheckerTexture_;
	sf::Texture whiteCheckerKingTexture_;
	sf::Texture blackCheckerTexture_;
	sf::Texture blackCheckerKingTexture_;
    
};


#endif