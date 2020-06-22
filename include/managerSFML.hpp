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
#include "checkerboard.hpp"

class SpriteChecker
{
public:
	sf::Sprite sprite;
	bool isRed = false;
	bool isKing = false;
};

class GUI
{
public:
    GUI();
    // Update what's on screen, with newBoard:
    void setBoard(const std::bitset<96> newBoard);
    // Get board from GUI:
    std::bitset<96> getCheckersFromGUI();
    // whether some human closed the window:
    bool isWindowOpen();
    // Let the player choose the first checker they want to move:
    int getFirstChecker(const bool isRedTeam, const bool validate=true);
    // Let the player keep moving it til it's done/they do something invalid:
    bool getNextChecker(const int prevCheckerID, const bool isRedTeam);
    // Highlight a checker: (for if a human player clicks on one)
    void highlightChecker(int checkerID);
    // Wrapper around the one that takes in an event:
    void checkQuitGUI();

private:
    // Check if someone pressed 'x', and quit out
    void checkQuitGUI(sf::Event event);
    // Load the initial textures from the images dir:
    bool loadTextures();
    // Convert ID (0-31) to x,y on checker board:
    const sf::Vector2f intToCords(uint i);
    // Take where the player clicked, and make it 0-31, or -1. 
    //     (Based on which checker):
    int MousePositionToInt(const sf::Vector2i locationClicked);
    // Update the checkers_ variable with newBoard:
    void checkersUpdate(const std::bitset<96> newBoard);
    // Update what's on screen, with checkers_:
    void windowUpdate();

private:
    // Window Stuff:
    sf::RenderWindow window_;

    // Sprite Stuff:
    std::vector<std::shared_ptr<SpriteChecker>> checkers_;
    sf::Sprite checkerboard_;

    // Texture Stuff:
	sf::Texture checkerboardTexture_;
	sf::Texture whiteCheckerTexture_;
	sf::Texture whiteCheckerKingTexture_;
	sf::Texture blackCheckerTexture_;
	sf::Texture blackCheckerKingTexture_;
    
};


#endif