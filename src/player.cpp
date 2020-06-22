
#include "../include/player.hpp"

Player::Player(const std::string &plr_type) :
	playerType_(plr_type),
	gameGUI_(nullptr)
{
	if(! ( playerType_ == "human" 		|| 
		   playerType_ == "random" 		|| 
		   playerType_ == "piece_count" || 
		   playerType_ == "AI" ))
	{
		std::cout << "UNKNOWN PLAYER PASSED - either 'human', 'random', 'piece_count', or 'AI' please." << std::endl;
		std::cout << "        (tried to create '" << plr_type << "' player)." << std::endl;
		return;
	}
}

void Player::setGUI(std::shared_ptr<GUI> &gameGUI){
	gameGUI_ = gameGUI;
}

std::bitset<96> Player::getMove(const std::bitset<96> &bitBoard, bool isRedTeam)
{
	if(playerType_ == "human")
	{
		if(gameGUI_ == nullptr){
			std::cout << " - ERROR: It's a human's turn, but the GUI was never passed to player... Quitting." << std::endl;
			return std::bitset<96>(0);
		}

		while(gameGUI_->isWindowOpen())
		{
			gameGUI_->setBoard(bitBoard);
			int checkerID = gameGUI_->getFirstChecker(isRedTeam);
			// If they click 'x', or off a space or something, do another lap:
			if(checkerID == -1){
				break;
			}
			gameGUI_->highlightChecker(checkerID);
			// This returns true (recursivly) for each combo jump:
			if( gameGUI_->getNextChecker(checkerID, isRedTeam) ){
				return gameGUI_->getCheckersFromGUI();
			}
		}
	}
	else if(playerType_ == "random")
	{
		using namespace std::chrono_literals;
		// Just so that it doesn't instantly finish moving. Might remove later:
		std::this_thread::sleep_for(2s);
		std::bitset<96> newBoard = CheckerBoardMoves(bitBoard, isRedTeam).getRandoMove();
		if (gameGUI_ != nullptr){
			gameGUI_->setBoard(newBoard);
		}
		return newBoard;
	}
	else if(playerType_ == "piece_count")
	{
		std::bitset<96> newBoard = Negamax(bitBoard, isRedTeam).getBestBoard();
		if (gameGUI_ != nullptr){
			gameGUI_->setBoard(newBoard);
		}
		return newBoard;
	}
	std::bitset<96> tmp(0);
	return tmp;
}

bool Player::validMovesRecursive(int prevCheckerID, const std::bitset<96> &oldMove, bool isRedTeam)
{
	CheckerBoardMoves endMoves(oldMove, isRedTeam, false);
	CheckerBoardMoves nextMove(oldMove, isRedTeam, true);

	int newCheckerID = gameGUI_->getNextChecker(prevCheckerID, isRedTeam);
	// If they click 'x', or off a space or something, do another lap:
	if(newCheckerID == -1){
		return false;
	}
	std::bitset<96> newMove = gameGUI_->getCheckersFromGUI();
	// If it IS the finishing board:
	if(endMoves.isValidBoard(newMove)){
		return true;
	}
	// If it LEADS to a finishing board:
	else if(nextMove.isValidBoard(newMove)){
		return validMovesRecursive(newCheckerID, newMove, isRedTeam);
	}
	// Else it's not a valid move:
	else{
		return false;
	}
}



