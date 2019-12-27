

#include "../include/player.hpp"


Player::Player(const std::string &plr_type, bool redTeam, sf::RenderWindow &window) :
	window_(window),
	isRedTeam_(redTeam),
	playerType_(plr_type)
{
	if(! ( playerType_ == "human" || 
		   playerType_ == "random" || 
		   playerType_ == "piece_count" || 
		   playerType_ == "AI"))
	{
		playerType_ = "unknown";
		std::cout << "UNKNOWN PLAYER PASSED - either 'human', 'random', 'piece_count', or 'AI' please." << std::endl;
		std::cout << "        --tried to create '" << playerType_ << "' player." << std::endl;
		window_.close();
		return;
	}
	// load file here?
}

std::bitset<96> Player::getMove(std::vector<std::unique_ptr<SpriteChecker>> &checkers)
{
	std::bitset<96> bestBoard;
	if(playerType_ == "human")
	{
		humanManager(checkers);
		return checkersToBoard(checkers);
	}
	else if(playerType_ == "random")
	{
		using namespace std::chrono_literals;
		// Just so that it doesn't instantly finish moving. Might remove later:
		std::this_thread::sleep_for(2s);
		std::bitset<96> bitboard = checkersToBoard(checkers);
		return CheckerBoardMoves(bitboard, isRedTeam_).getRandoMove();
	}
	else if(playerType_ == "piece_count")
	{
		std::bitset<96> bitboard = checkersToBoard(checkers);
		return Negamax(bitboard, isRedTeam_).getBestBoard();
	}
	std::bitset<96> tmp(0);
	return tmp;
}


void Player::humanManager(std::vector<std::unique_ptr<SpriteChecker>> &checkers)
{
	sf::Event evnt;
	while( window_.isOpen() )
	{
		if(window_.waitEvent(evnt))
		{
			switch(evnt.type)
			{
				case sf::Event::Closed:{
					window_.close();
					return;
				}break;

				case sf::Event::MouseButtonPressed:{
					int position = MousePositionToInt(sf::Mouse::getPosition(window_));
					// if not a valid click
					if( position != -1 && checkers[position] != nullptr && checkers[position]->isRed == isRedTeam_)
					{
						checkers[position]->sprite.setColor(sf::Color(200,200,200,200));
						window_.draw(checkers[position]->sprite);
						window_.display();
						if(JumpingRecursively(position, checkers))
						{
							return;
						}
						// else move wasn't valid, reset and try again
						checkers[position]->sprite.setColor(sf::Color(255,255,255,255));
						window_.draw(checkers[position]->sprite);
						window_.display();
					}
				}break;
			}
		}
	}
}

bool Player::JumpingRecursively(int pieceIndex, std::vector<std::unique_ptr<SpriteChecker>> &checkers)
{
	// To help check if you finished moving:
	CheckerBoardMoves allMoves(checkersToBoard(checkers), isRedTeam_, false);
	CheckerBoardMoves oneLayer(checkersToBoard(checkers), isRedTeam_, true);

	while(window_.isOpen())
	{
		sf::Event evnt;
		if(window_.waitEvent(evnt))
		{
			switch(evnt.type)
			{
				case sf::Event::Closed:{
					window_.close();
					return true; // true so that it breaks out of everything
				}break;

				case sf::Event::MouseButtonPressed:{
					int position = MousePositionToInt(sf::Mouse::getPosition(window_));
					// If you clicked somewhere invalid, just ignore it:
					if( position == -1 || checkers[position] != nullptr )
					{
						return false;
					}

					std::unique_ptr<SpriteChecker> swapStorage;
					swapCheckers(pieceIndex, position, swapStorage, checkers);
					bool wasKing = checkers[position]->king;

					if((  isRedTeam_ && (position == 28 || position == 29 || position == 30 || position == 31)) ||
					   ( !isRedTeam_ && (position == 0  || position == 1  || position == 2  || position == 3 )) )
					{
						checkers[position]->king = true;
					}

					std::bitset<96> bitboard = checkersToBoard(checkers);
					if( allMoves.isValidBoard(bitboard) )
					{
						return true;
					}
					if( oneLayer.isValidBoard(bitboard) )
					{
						if(JumpingRecursively(position, checkers) == true)
						{
							return true;
						}
					}
					checkers[position]->king = wasKing;
					swapCheckers(position, pieceIndex, swapStorage, checkers);
					return false;

				}break;
			}
		}
	}
}

// takes the square the user clicked (x,y) and converts it to:
// 		0-31 if valid checker square
// 		-1 if not
int Player::MousePositionToInt(sf::Vector2i locationClicked)
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

void Player::swapCheckers(int f, int s, std::unique_ptr<SpriteChecker> &swapStorage, std::vector<std::unique_ptr<SpriteChecker>> &checkers)
{
	if(! (checkers[s] == nullptr && checkers[f]->isRed == isRedTeam_)){
		return;
	}
	std::swap(checkers[f], checkers[s]);
	for(uint i=0; i<2; ++i)
	{
		if((*RED_JUMP_BOARD)[f][i] == s)
		{
			std::swap(checkers[(*RED_MOVE_BOARD)[f][i]], swapStorage);
			return;
		}
		if((*BLACK_JUMP_BOARD)[f][i] == s)
		{
			std::swap(checkers[(*BLACK_MOVE_BOARD)[f][i]], swapStorage);
			return;
		}
	}
}








//////////////////////////////////////////////////////////////
////////////    ----- "Helper" Functions -----    ////////////
//////////////////////////////////////////////////////////////

std::bitset<96> checkersToBoard(std::vector<std::unique_ptr<SpriteChecker>> &checkers)
{
	std::bitset<96> returnMe(0);
	for(uint i=0; i<32; ++i)
	{
		if(checkers[i] == nullptr)
			continue;
		if(checkers[i]->isRed == true && checkers[i]->king == true)
		{
			returnMe[i] = 1;
			returnMe[i+32] = 1;
		}
		else if(checkers[i]->isRed == true && checkers[i]->king == false)
		{
			returnMe[i] = 0;
			returnMe[i+32] = 1;
		}
		else if(checkers[i]->isRed == false && checkers[i]->king == true)
		{
			returnMe[i] = 1;
			returnMe[i+64] = 1;
		}
		else if(checkers[i]->isRed == false && checkers[i]->king == false)
		{
			returnMe[i] = 0;
			returnMe[i+64] = 1;
		}
	}
	return returnMe;
}

// Goes from the squares id to x-y values, then multiplies them by
//		a width of a single square
const sf::Vector2f intToCords(uint i)
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