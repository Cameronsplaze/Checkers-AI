
#include "../include/managerSFML.hpp"



GUI::GUI()
    : window_(sf::VideoMode(648,648), "A not-yet-so-great Checkers AI", sf::Style::Titlebar | sf::Style::Close)
{
	window_.setVerticalSyncEnabled(true);
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
}

std::bitset<96> GUI::getCheckersFromGUI()
{
	std::bitset<96> bitBoard(0);
	for(uint i=0; i<32; ++i)
	{
		if(checkers_[i] == nullptr){
			continue;
		}
		if(checkers_[i]->isRed){
			if(checkers_[i]->isKing){
				bitBoard[i+32] = 1; // red
				bitBoard[i] = 1; //king
			}
			else{
				bitBoard[i+32] = 1; // red
			}
		}
		else{
			if(checkers_[i]->isKing){
				bitBoard[i+64] = 1; // black
				bitBoard[i] = 1; //king
			}
			else{
				bitBoard[i+64] = 1; // black
			}

		}
	}
	return bitBoard;
}

bool GUI::isWindowOpen()
{
    return window_.isOpen();
}

// Let the player choose the first checker they want to move:
int GUI::getFirstChecker(const bool isRedTeam, const bool validate)
{
	sf::Event event;
	while(window_.waitEvent(event))
	{
		// This handles checking if the user quits out:
		checkQuitGUI(event);
		// This handles clicking on a checker:
		switch(event.type)
		{
			case sf::Event::MouseButtonPressed:
				int ID = MousePositionToInt(sf::Mouse::getPosition(window_));
				if(validate && (ID == -1 || checkers_[ID] == nullptr || checkers_[ID]->isRed != isRedTeam)){
					break;
				}
				return ID;
		}
	}
	// This gets hit if waitEvent hits an error and returns false:
	return -1;
}

// Let the player keep moving it til it's done/they do something invalid:
bool GUI::getNextChecker(const int oldCheckerID, const bool isRedTeam)
{
	std::bitset<96> originalBoard = getCheckersFromGUI();
	// For checking if you make a valid move, or 
	//	  if you make one that might LEAD to a valid move:
	CheckerBoardMoves endMoves(originalBoard, isRedTeam, false);
	CheckerBoardMoves nextMove(originalBoard, isRedTeam, true);
	
	// Get a new checker (false skips the validate step, so it can be a blank square):
	int newCheckerID = getFirstChecker(isRedTeam, false);
	if(newCheckerID == -1){
		return false;
	}
	// After you swap them, see if the checker in the new location should become a king:
	checkers_[oldCheckerID].swap(checkers_[newCheckerID]);
	bool wasKing = checkers_[newCheckerID]->isKing;
	if((  isRedTeam && (newCheckerID == 28 || newCheckerID == 29 || newCheckerID == 30 || newCheckerID == 31)) ||
		( !isRedTeam && (newCheckerID == 0  || newCheckerID == 1  || newCheckerID == 2  || newCheckerID == 3 )) ){
		checkers_[newCheckerID]->isKing = true;
	}

	// If the checker 'jumped' something, delete it:
	std::shared_ptr<LookupTable> teamMoveBoard = (isRedTeam) ? RED_MOVE_BOARD : BLACK_MOVE_BOARD;
	std::shared_ptr<LookupTable> teamJumpBoard = (isRedTeam) ? RED_JUMP_BOARD : BLACK_JUMP_BOARD;
	std::shared_ptr<LookupTable> oppMoveBoard = (isRedTeam) ? BLACK_MOVE_BOARD : RED_MOVE_BOARD;
	std::shared_ptr<LookupTable> oppJumpBoard = (isRedTeam) ? BLACK_JUMP_BOARD : RED_JUMP_BOARD;
	// For 0=left, 1=right. (Check both sides):
	for(uint i=0; i<2; ++i)
	{
		// If you jumped a checker:
		if( (*teamJumpBoard)[oldCheckerID][i] == newCheckerID ){
			int jumpedOverID = (*teamMoveBoard)[oldCheckerID][i];
			checkers_[jumpedOverID] = nullptr;
		}
		// Incase it's a king that did the jumping, do the other side too:
		else if( (*oppJumpBoard)[oldCheckerID][i] == newCheckerID ){
			int jumpedOverID = (*oppMoveBoard)[oldCheckerID][i];
			checkers_[jumpedOverID] = nullptr;
		}
	}

	// Now compare the current board, to the possible valid boards:
	std::bitset<96> newBoard = getCheckersFromGUI();
	setBoard(newBoard); // <-- To redraw the new move
	highlightChecker(newCheckerID);

	// If it IS the finishing board:
	if(endMoves.isValidBoard(newBoard)){
		setBoard(newBoard); // <-- To redraw the new move
		return true; // You got one, you're done!
	}
	// If it LEADS to a finishing board:
	else if(nextMove.isValidBoard(newBoard) && getNextChecker(newCheckerID, isRedTeam)){
		return true;
	}
	// Else they made a invalid move:
	else{
		return false;
	}
}

void GUI::highlightChecker(int checkerID)
{
	if(checkerID == -1 || checkers_[checkerID] == nullptr){
		return;
	}
	// Handle it's highlighting/displaying:
	checkers_[checkerID]->sprite.setColor(sf::Color(200,200,200,200));
	windowUpdate();
}

// Goes through ALL events in queue:
void GUI::checkQuitGUI()
{
    sf::Event event;
    while(window_.pollEvent(event))
    {
		checkQuitGUI(event);
    }
}

// Checks the CURRENT event in queue:
void GUI::checkQuitGUI(sf::Event event)
{
	switch(event.type)
	{
		// They press the 'x' button:
		case sf::Event::Closed:
			std::cout << "CLOSING WINDOW: User pressed 'x'." << std::endl;
			window_.close();
			return;
		case sf::Event::KeyPressed:
			if(event.key.code == sf::Keyboard::Escape){
				std::cout << "CLOSING WINDOW: User pressed escape." << std::endl;
				window_.close();
				return;
			}
	}
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

int GUI::MousePositionToInt(const sf::Vector2i locationClicked)
{
	uint x = (locationClicked.x - 26)/76; // 0-7
	uint y = (locationClicked.y - 26)/76; // 0-7

	if(x%2 == y%2 || x > 7 || y > 7) // if you click on the boarder, x y can "be" >7
		return -1;

	if(x%2 == 0)
		return x/2 + y*4;
	else// x%2 == 1
		return (x-1)/2 + y*4;
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
			checkers_[i] = std::make_shared<SpriteChecker>(checker);
		}
		// if red checker
		if(newBoard[i+32] == 1 && newBoard[i+64] == 0)
		{
			if(newBoard[i] == 0)
			{
				checkers_[i]->sprite.setTexture(whiteCheckerTexture_);
				checkers_[i]->sprite.setPosition(intToCords(i));
				checkers_[i]->isRed = true;
				checkers_[i]->isKing = false;
			}
			else
			{
				checkers_[i]->sprite.setTexture(whiteCheckerKingTexture_);
				checkers_[i]->sprite.setPosition(intToCords(i));
				checkers_[i]->isRed = true;
				checkers_[i]->isKing = true;
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
				checkers_[i]->isKing = false;
			}
			else
			{
				checkers_[i]->sprite.setTexture(blackCheckerKingTexture_);
				checkers_[i]->sprite.setPosition(intToCords(i));
				checkers_[i]->isRed = false;
				checkers_[i]->isKing = true;
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
		window_.draw(checkers_[i]->sprite);
	}
	window_.display();
}



