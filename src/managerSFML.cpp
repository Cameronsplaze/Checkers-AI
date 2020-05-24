
#include "../include/managerSFML.hpp"



GUI::GUI()
    : window_(sf::VideoMode(648,648), "A not-yet-so-great Checkers AI", sf::Style::Titlebar | sf::Style::Close)
{
    window_.setFramerateLimit(30);
	// load textures - If fails, can't do anything
	bool success = loadTextures();
	if(!success){
		return;
	}
}

void GUI::setBoard(const std::bitset<96> newBoard)
{
    checkersUpdate(newBoard);
    windowUpdate();
    processEvents();
}

bool GUI::isWindowOpen()
{
    return window_.isOpen();
}

bool GUI::loadTextures()
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


// Goes from the squares id (0-31), to x-y values. Then multiplies 
//      them by a width of a single square
const sf::Vector2f GUI::intToCords(uint i)
{
	uint x1;
	uint y1 = i/4;

	i = i%8;
	if(i > 3)
		x1 = 2*i-8;
	else // if x1%8 <= 3
		x1 = 2*i+1;

	float x = x1*76.0 + 26.0;
	float y = y1*76.0 + 26.0;
	return sf::Vector2f(x, y);
}

void GUI::checkersUpdate(const std::bitset<96> newBoard)
{
	checkers_.clear();
	checkers_.resize(32);
	for(uint i=0; i<32; ++i)
	{
		// if no checker
		if(newBoard[i+32] == 0 && newBoard[i+64] == 0)
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
		if(newBoard[i+32] == 1 && newBoard[i+64] == 0)
		{
			if(newBoard[i] == 0)
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
		else if(newBoard[i+32] == 0 && newBoard[i+64] == 1)
		{
			if(newBoard[i] == 0)
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

void GUI::windowUpdate()
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

void GUI::processEvents()
{
    sf::Event event;
    while(window_.pollEvent(event))
    {
        switch(event.type)
        {
            // They press the 'x' button:
            case sf::Event::Closed:
				std::cout << "CLOSING WINDOW: User pressed 'x'." << std::endl;
				window_.close();
				return;        
        }
    }
}







