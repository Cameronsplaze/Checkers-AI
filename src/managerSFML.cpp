
#include "../include/managerSFML.hpp"





CheckerboardGUI::CheckerboardGUI(const std::string &player1, const std::string &player2)
	: window_(sf::VideoMode(648,648), "A not-yet-so-great Checkers AI", sf::Style::Titlebar | sf::Style::Close),
	  player1_(player1, true, window_, event_),
	  player2_(player2, false, window_, event_),
	  isPlayer1Turn_(NUM_RANDO_TURNS % 2 == 0)
{
	window_.setFramerateLimit(30);

	// load textures - If fails, can't do anything
	bool success = loadTextures();
	if(!success){
		return;
	}
	// setup checkers_ vec for updating board

	updateSprites(getStartBoard());
	mainGameloop();
}

bool CheckerboardGUI::loadTextures()
{
	const std::string CBimage = "checkerboard.PNG";
	const std::string WCimage = "whitechecker.png";
	const std::string WCKimage = "whitecheckerking.png";
	const std::string BCimage = "blackchecker.png";
	const std::string BCKimage = "blackcheckerking.png";

	// Load all the textures
	if(!checkerboardTexture_.loadFromFile(PATH_TO_IMAGES + CBimage)){
		std::cout << "COULD NOT LOAD '" << CBimage << "'' from '" << PATH_TO_IMAGES << "'" << std::endl;
		return false;
	}
	if(!whiteCheckerTexture_.loadFromFile(PATH_TO_IMAGES + WCimage)){
		std::cout << "COULD NOT LOAD '" << WCimage << "' from '" << PATH_TO_IMAGES << "'" << std::endl;
		return false;
	}
	if(!whiteCheckerKingTexture_.loadFromFile(PATH_TO_IMAGES + WCKimage)){
		std::cout << "COULD NOT LOAD '" << WCKimage << "' from '" << PATH_TO_IMAGES << "'" << std::endl;
		return false;
	}
	if(!blackCheckerTexture_.loadFromFile(PATH_TO_IMAGES + BCimage)){
		std::cout << "COULD NOT LOAD '" << BCimage << "' from '" << PATH_TO_IMAGES << "'" << std::endl;
		return false;
	}
	if(!blackCheckerKingTexture_.loadFromFile(PATH_TO_IMAGES + BCKimage)){
		std::cout << "COULD NOT LOAD '" << BCKimage << "' from '" << PATH_TO_IMAGES << "'" << std::endl;
		return false;
	}
	checkerboard_.setTexture(checkerboardTexture_);


	// if you made it this far... all is good
	return true;
}

void CheckerboardGUI::updateSprites(std::bitset<96> startBoard)
{
	checkers_.clear();
	checkers_.resize(32);
	for(uint i=0; i<32; ++i)
	{
		// if no checker
		if(startBoard[i+32] == 0 && startBoard[i+64] == 0)
		{
			checkers_[i] = nullptr;
			continue;
		}
		else
		{
			SpriteChecker checker;
			checkers_[i] = std::make_unique<SpriteChecker>(checker);
		}
		// if red checker
		if(startBoard[i+32] == 1 && startBoard[i+64] == 0)
		{
			if(startBoard[i] == 0)
			{
				checkers_[i]->sprite.setTexture(whiteCheckerTexture_);
				checkers_[i]->sprite.setPosition(intToCords(i));
				checkers_[i]->isRed = true;
				checkers_[i]->king = false;
			}
			else
			{
				checkers_[i]->sprite.setTexture(whiteCheckerKingTexture_);
				checkers_[i]->sprite.setPosition(intToCords(i));
				checkers_[i]->isRed = true;
				checkers_[i]->king = true;
			}
		}
		// if black checker
		else if(startBoard[i+32] == 0 && startBoard[i+64] == 1)
		{
			if(startBoard[i] == 0)
			{
				checkers_[i]->sprite.setTexture(blackCheckerTexture_);
				checkers_[i]->sprite.setPosition(intToCords(i));
				checkers_[i]->isRed = false;
				checkers_[i]->king = false;
			}
			else
			{
				checkers_[i]->sprite.setTexture(blackCheckerKingTexture_);
				checkers_[i]->sprite.setPosition(intToCords(i));
				checkers_[i]->isRed = false;
				checkers_[i]->king = true;
			}
		}
	}
}



void CheckerboardGUI::mainGameloop()
{
	while (window_.isOpen())
	{

		while(window_.pollEvent(event_))
		{
			// If they closed the window:
			if(event_.type == sf::Event::Closed){
				std::cout << "CLOSING WINDOW, main GameLoop" << std::endl;
				window_.close();
				return;
			}
			else
			{
				windowUpdate();
				if(isPlayer1Turn_){
					updateSprites(player1_.getMove(checkers_));
					isPlayer1Turn_ = false;
				}
				else{
					updateSprites(player2_.getMove(checkers_));
					isPlayer1Turn_ = true;
				}
			}
		}
	}
	std::cout << "Exited main game loop" << std::endl;
}


void CheckerboardGUI::windowUpdate()
{
	// handle drawing stuff:
	window_.clear();
	window_.draw(checkerboard_);
	for(uint i=0; i<checkers_.size(); ++i)
	{
		if(checkers_[i] == nullptr)
			continue;
		window_.draw((*checkers_[i]).sprite);
	}
	window_.display();
}






